#include <nexilis/boost_tcp/socket.hh>
#include <nexilis/logger/log.hh>

#include <boost/asio/buffers_iterator.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/asio/write.hpp>

#include <cstdint>

namespace nexilis::boost_tcp
{

Socket::Socket(boost::asio::io_context& io) noexcept
    : NxClass("boost_tcp::Socket-server"),
      m_socket(std::make_shared<boost::asio::ip::tcp::socket>(io))
{
}

Socket::Socket(boost::asio::io_context& io, const std::string& host, uint16_t port) noexcept
    : NxClass("boost_tcp::Socket-client"),
      m_socket(std::make_shared<boost::asio::ip::tcp::socket>(io)),
      m_endpointId("client:" + host + ":" + std::to_string(port))
{
}

Socket::Socket(Socket&& other) noexcept
    : NxClass(std::move(other)),
      m_socket(std::move(other.m_socket)),
      m_connected(other.m_connected.load()),
      m_endpointId(std::move(other.m_endpointId))
{
    other.m_connected.store(false);
}

Socket& Socket::operator=(Socket&& other) noexcept
{
    if (this != &other)
    {
        static_cast<NxClass&>(*this) = static_cast<NxClass&&>(other);
        std::lock_guard<std::mutex> lock(m_mutex);
        std::lock_guard<std::mutex> other_lock(other.m_mutex);

        m_socket = std::move(other.m_socket);
        m_connected = m_connected.load();
        m_endpointId = std::move(other.m_endpointId);
        other.m_connected.store(false);
    }
    return *this;
}

bool Socket::connect(const std::string& host, uint16_t port)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_connected)
    {
        return true;
    }

    try
    {
        boost::asio::ip::tcp::resolver resolver(m_socket->get_executor());
        auto endpoints = resolver.resolve(host, std::to_string(port));

        boost::asio::connect(*m_socket, endpoints);

        m_endpointId = "client:" + host + ":" + std::to_string(port);
        m_connected = true;
        setSocketOptions();
        return true;
    }
    catch (...)
    {
        closeInternal();
        return false;
    }
}

void Socket::assign(boost::asio::ip::tcp::socket&& socket)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    *m_socket = std::move(socket);
    m_endpointId = "server:" + std::to_string(m_socket->local_endpoint().port());
    m_connected = m_socket->is_open();
    setSocketOptions();
}

bool Socket::send(const nx_data& data)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    if (!m_connected)
    {
        return false;
    }

    try
    {
        size_t written = boost::asio::write(
                *m_socket,
                boost::asio::buffer(data),
                boost::asio::transfer_all());
        return written == data.size();
    }
    catch (...)
    {
        closeInternal();
        return false;
    }
}

bool Socket::receive(nx_data& data, size_t timeout_ms)
{
    // The socket mutex must NOT be held while waiting for data in select().
    // The server can call send() on this socket from another client's thread
    // (e.g. relaying a room command to this client). Holding the mutex for the
    // whole select() duration starves that thread indefinitely and deadlocks
    // the server. Check the connection state briefly, then wait for data
    // without holding the mutex.
    if (!isOpen())
    {
        return false;
    }

    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(m_socket->native_handle(), &read_fds);

    timeval timeout;
    timeout.tv_sec = timeout_ms / 1000;
    timeout.tv_usec = (timeout_ms % 1000) * 1000;

    int result = select(m_socket->native_handle() + 1, &read_fds, nullptr, nullptr,
                        timeout_ms > 0 ? &timeout : nullptr);

    if (result <= 0)
    {
        return false; // Timeout or error
    }

    try
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (!m_connected)
        {
            return false;
        }

        // select() reported data available, so these blocking reads complete
        // promptly and never stall the mutex for long.
        boost::system::error_code ec;

        // Read 4-byte length prefix (big-endian uint32_t)
        uint8_t len_buf[4];
        boost::asio::read(*m_socket,
                          boost::asio::buffer(len_buf),
                          boost::asio::transfer_exactly(4), ec);

        if (ec)
        {
            closeInternal();
            return false;
        }

        uint32_t payload_size = (static_cast<uint32_t>(len_buf[0]) << 24) |
                                (static_cast<uint32_t>(len_buf[1]) << 16) |
                                (static_cast<uint32_t>(len_buf[2]) << 8) |
                                (static_cast<uint32_t>(len_buf[3]));

        if (payload_size == 0)
        {
            data.clear();
            return true;
        }

        // Read exactly payload_size bytes
        boost::asio::streambuf buf;
        boost::asio::read(*m_socket, buf,
                          boost::asio::transfer_exactly(payload_size), ec);

        if (ec)
        {
            closeInternal();
            return false;
        }

        auto bufs = buf.data();
        data.assign(
                boost::asio::buffers_begin(bufs),
                boost::asio::buffers_end(bufs));
        return true;
    }
    catch (...)
    {
        closeInternal();
        return false;
    }
}

void Socket::close()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    closeInternal();
}

void Socket::closeInternal()
{
    if (!m_connected)
    {
        return;
    }

    boost::system::error_code ec;
    // Cancel ongoing async operations.
    ec = m_socket->cancel(ec);
    if (ec)
    {
        Log::error(header(), "Error cancelling socket operations");
    }

    ec = m_socket->shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
    if (ec)
    {
        Log::error(header(), "Error shutting down socket");
    }

    ec = m_socket->close(ec);
    if (ec)
    {
        Log::error(header(), "Error closing socket");
    }
    m_connected = false;
}

void Socket::forceClose()
{
    std::unique_lock<std::mutex> lock(m_mutex, std::try_to_lock);

    if (!lock.owns_lock())
    {
        boost::system::error_code ec;
        if (m_socket && m_socket->is_open())
        {
            ec = m_socket->cancel(ec);
            if (ec)
            {
                Log::error(header(), "Error cancelling socket");
            }

            ec = m_socket->close(ec);
            if (ec)
            {
                Log::error(header(), "Error closing socket");
            }
            return;
        }

        if (m_connected && m_socket && m_socket->is_open())
        {
            boost::system::error_code close_ec;
            close_ec = m_socket->cancel(close_ec);
            if (close_ec)
            {
                Log::error(header(), "Error cancelling socket");
            }

            close_ec = m_socket->shutdown(boost::asio::ip::tcp::socket::shutdown_both, close_ec);
            if (close_ec)
            {
                Log::error(header(), "Error shutting down socket");
            }

            close_ec = m_socket->close(close_ec);
            if (close_ec)
            {
                Log::error(header(), "Error closing socket");
            }

            m_connected = false;
        }
    }
}

std::string Socket::getEndpointId() const
{
    return m_endpointId;
}

bool Socket::isOpen() const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_socket && m_socket->is_open();
}

std::string Socket::getRemoteAddress() const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (!m_socket || !m_socket->is_open())
    {
        return "disconnected";
    }

    try
    {
        return m_socket->remote_endpoint().address().to_string();
    }
    catch (...)
    {
        return "unknown";
    }
}

void Socket::cancelAllSocketOperations()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_connected && m_socket && m_socket->is_open())
    {
        boost::system::error_code ec;
        ec = m_socket->cancel(ec);
        if (ec)
        {
            Log::error(header(), "Error cancelling socket operations: ", ec.message());
        }
    }
}

void Socket::setSocketOptions()
{
    if (!m_connected)
    {
        return;
    }

    boost::system::error_code ec;
    ec = m_socket->set_option(boost::asio::ip::tcp::no_delay(true), ec);
    if (ec)
    {
        Log::error(header(), "Error setting option \"no_delay\"");
    }

    ec = m_socket->set_option(boost::asio::socket_base::keep_alive(true), ec);
    if (ec)
    {
        Log::error(header(), "Error setting option \"keep_alive\"");
    }

    ec = m_socket->set_option(boost::asio::socket_base::linger(true, 5), ec);
    if (ec)
    {
        Log::error(header(), "Error setting option \"linger\"");
    }
}

} // namespace nexilis::boost_tcp
