#include <nexilis/client/protocol/nxboost/tcp_client.hh>
#include <nexilis/logger/log.hh>
#include <nexilis/ports.hh>

#include <boost/asio/bind_executor.hpp>
#include <boost/asio/buffers_iterator.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/read_until.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/asio/write.hpp>

namespace nexilis::client::nxboost
{

TCPClient::TCPClient(ClientAPI& api)
    : NxClass("client::nxboost::TCPClient"),
      Protocol(),
      ClientProtocol(&api),
      m_stopped(std::make_unique<std::atomic<bool>>(false)),
      m_ioContext(std::make_shared<boost::asio::io_context>()),
      m_strand(std::make_shared<boost::asio::io_context::strand>(*m_ioContext)),
      m_workGuard(std::make_unique<boost::asio::executor_work_guard<boost::asio::io_context::executor_type>>(
              boost::asio::make_work_guard(*m_ioContext))),
      m_socket(),
      m_resolver(*m_ioContext),
      m_sendMutex(std::make_shared<std::mutex>()),
      m_receiveMutex(std::make_shared<std::mutex>()),
      m_portSwitchingMutex(std::make_shared<std::mutex>()),
      m_pendingSendsMutex(std::make_shared<std::mutex>())
{
    auto new_socket = std::make_shared<boost::asio::ip::tcp::socket>(*m_ioContext);
    storeSocket(new_socket);

    // Verify all shared pointers were created.
    if (!m_sendMutex || !m_receiveMutex || !m_portSwitchingMutex)
    {
        throw std::runtime_error("Failed to initialize synchronization objects");
    }
}

TCPClient::TCPClient(TCPClient&& other)
    : NxClass(std::move(other)),
      Protocol(std::move(other)),
      ClientProtocol(std::move(other)),
      m_ioContextThread(std::move(other.m_ioContextThread)),
      m_portSwitchingThread(std::move(other.m_portSwitchingThread)),
      m_stopped(std::move(other.m_stopped)),
      m_ioContext(std::move(other.m_ioContext)),
      m_strand(std::move(other.m_strand)),
      m_workGuard(std::move(other.m_workGuard)),
      m_socket(other.m_socket.load()),
      m_resolver(std::move(other.m_resolver)),
      m_sendMutex(std::move(other.m_sendMutex)),
      m_receiveMutex(std::move(other.m_receiveMutex)),
      m_portSwitchingMutex(std::move(other.m_portSwitchingMutex)),
      m_serverPort(std::move(other.m_serverPort))
{
    other.m_socket.store(nullptr);

    other.m_stopped.reset();
    other.m_strand.reset();
    other.m_workGuard.reset();
    other.m_sendMutex.reset();
    other.m_receiveMutex.reset();
    other.m_portSwitchingMutex.reset();
}

TCPClient& TCPClient::operator=(TCPClient&& other)
{
    if (this != &other)
    {
        static_cast<NxClass&>(*this) = static_cast<NxClass&&>(other);
        static_cast<Protocol&>(*this) = static_cast<Protocol&&>(other);
        static_cast<ClientProtocol&>(*this) = static_cast<ClientProtocol&&>(other);

        if (m_ioContextThread.joinable())
        {
            m_ioContextThread.join();
        }
        if (m_portSwitchingThread.joinable())
        {
            m_portSwitchingThread.join();
        }

        m_ioContextThread = std::move(other.m_ioContextThread);
        m_portSwitchingThread = std::move(other.m_portSwitchingThread);
        m_stopped = std::move(other.m_stopped);
        m_ioContext = std::move(other.m_ioContext);
        m_strand = std::move(other.m_strand);
        m_workGuard = std::move(other.m_workGuard);
        m_socket.store(other.m_socket.load());
        m_resolver = std::move(other.m_resolver);
        m_sendMutex = std::move(other.m_sendMutex);
        m_receiveMutex = std::move(other.m_receiveMutex);
        m_portSwitchingMutex = std::move(other.m_portSwitchingMutex);
        m_serverPort = std::move(other.m_serverPort);

        other.m_socket.store(nullptr);
        other.m_stopped.reset();
        other.m_workGuard.reset();
        other.m_strand.reset();
        other.m_sendMutex.reset();
        other.m_receiveMutex.reset();
        other.m_portSwitchingMutex.reset();
    }
    return *this;
}

TCPClient::~TCPClient()
{
    stop();
    m_workGuard.reset();
}

void TCPClient::stop()
{
    if (m_stopped && m_stopped->exchange(true))
    {
        return;
    }

    if (m_receiveMutex && m_sendMutex)
    {
        std::lock_guard<std::mutex> recvLock(*m_receiveMutex);
        std::lock_guard<std::mutex> sendLock(*m_sendMutex);
        auto current_socket = m_socket.load();
        if (current_socket && current_socket->is_open())
        {
            boost::system::error_code ec;
            ec = current_socket->cancel(ec);
            if (ec)
            {
                Log::error(header(), "Error cancelling socket");
            }
            ec = current_socket->shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
            if (ec)
            {
                Log::error(header(), "Error shutting down socket");
            }
            ec = current_socket->close(ec);
            if (ec)
            {
                Log::error(header(), "Error closing socket");
            }
        }
    }

    m_workGuard.reset();

    if (m_ioContext)
    {
        m_ioContext->stop();
        Log::debug(header(), "io_context stopped");
    }

    if (m_portSwitchingThread.joinable())
    {
        m_portSwitchingThread.join();
        Log::debug(header(), "port switchingthread stopped");
    }

    if (m_ioContextThread.joinable())
    {
        m_ioContextThread.join();
        Log::debug(header(), "io_contextThread stopped");
    }

    if (m_pendingSendsMutex)
    {
        std::lock_guard<std::mutex> lock(*m_pendingSendsMutex);
        for (auto& pair : m_pendingSends)
        {
            try
            {
                pair.second->set_exception(
                        std::make_exception_ptr(std::runtime_error("Connection closed")));
            }
            catch (...)
            {
            }
        }
        m_pendingSends.clear();
    }
}

void TCPClient::sendMessage(const nx_data& message)
{
    send(message);
}

void TCPClient::sendMessage(const nx_data& message, const std::function<void()>& callback)
{
    sendMessageWithCallback(message, callback);
}

bool TCPClient::connectToServer()
{
    if (getProtocolStatus() != ProtocolStatus::undefined)
    {
        Log::error(header(), "ProtocolStatus is not undefined when trying the first connection");
        return false;
    }

    auto current_socket = m_socket.load();
    try
    {
        updateProtocolStatus(ProtocolStatus::connecting);
        m_serverPort = Ports::getBoostTCPPort();
        auto endpoints = m_resolver.resolve(getClientAPI()->getBoostTCPServerAddress(), std::to_string(m_serverPort));

        boost::system::error_code ec;
        boost::asio::connect(*current_socket, endpoints, ec);

        if (ec)
        {
            Log::error(header(), "Cannot connect in ConnectToServer");
            return false;
        }
        else
        {
            Log::debug(header(), "Initial connection success");
            updateProtocolStatus(ProtocolStatus::connected);
            return true;
        }
    }
    catch (...)
    {
        updateProtocolStatus(ProtocolStatus::error);
        current_socket->close();
        return false;
    }
}

bool TCPClient::send(const nx_data& data)
{
    if (getProtocolStatus() != ProtocolStatus::connected || m_stopped->load())
    {
        Log::warning(header(), "Cannot send in state: ", getProtocolStatusString());
        return false;
    }

    std::shared_ptr<boost::asio::ip::tcp::socket> current_socket = loadSocket();
    if (!current_socket || !current_socket->is_open())
    {
        Log::error(header(), "Socket is invalid or not open");
        return false;
    }

    // Extract message ID for this send
    uint64_t messageId = 0;
    if (data.size() >= 16)
    {
        std::memcpy(&messageId, data.data() + 8, sizeof(uint64_t));
    }

    try
    {
        boost::asio::async_write(*current_socket, boost::asio::buffer(data),
                                 boost::asio::bind_executor(*m_strand,
                                                            [this, current_socket, messageId](const boost::system::error_code& ec, std::size_t size)
                                                            {
                                                                if (!ec)
                                                                {
                                                                    Log::info(header(), "Message sent: ", size, " bytes, ID: ", messageId);

                                                                    // Fulfill the promise when write completes
                                                                    if (messageId != 0)
                                                                    {
                                                                        std::lock_guard<std::mutex> lock(*m_pendingSendsMutex);
                                                                        auto it = m_pendingSends.find(messageId);
                                                                        if (it != m_pendingSends.end())
                                                                        {
                                                                            it->second->set_value();
                                                                            m_pendingSends.erase(it);
                                                                        }
                                                                    }
                                                                }
                                                                else
                                                                {
                                                                    Log::error(header(), "Async write error: ", ec.message());

                                                                    // Fulfill with exception on error
                                                                    if (messageId != 0)
                                                                    {
                                                                        std::lock_guard<std::mutex> lock(*m_pendingSendsMutex);
                                                                        auto it = m_pendingSends.find(messageId);
                                                                        if (it != m_pendingSends.end())
                                                                        {
                                                                            it->second->set_exception(
                                                                                    std::make_exception_ptr(
                                                                                            std::runtime_error("Send failed: " + ec.message())));
                                                                            m_pendingSends.erase(it);
                                                                        }
                                                                    }
                                                                }
                                                            }));
        return true;
    }
    catch (const std::exception& e)
    {
        Log::error(header(), "Exception during async_write: ", e.what());

        // Clean up promise on exception
        if (messageId != 0)
        {
            std::lock_guard<std::mutex> lock(*m_pendingSendsMutex);
            auto it = m_pendingSends.find(messageId);
            if (it != m_pendingSends.end())
            {
                it->second->set_exception(std::current_exception());
                m_pendingSends.erase(it);
            }
        }
        return false;
    }
}

std::future<void> TCPClient::sendMessageAsync(const nx_data& message)
{
    // Extract message ID (second 8 bytes)
    if (message.size() < 16)
    {
        Log::error(header(), "Message too small");
        std::promise<void> emptyPromise;
        emptyPromise.set_exception(
                std::make_exception_ptr(std::runtime_error("Message too small")));
        return emptyPromise.get_future();
    }

    uint64_t messageId = 0;
    std::memcpy(&messageId, message.data() + 8, sizeof(uint64_t));

    auto promise = std::make_shared<std::promise<void>>();
    auto future = promise->get_future();

    {
        std::lock_guard<std::mutex> lock(*m_pendingSendsMutex);
        m_pendingSends[messageId] = promise;
    }

    // Send the message (this will trigger async write)
    send(message);

    return future;
}

void TCPClient::handlePortSwitch()
{
    if (!m_portSwitchingMutex || !m_portSwitchingMutex.get())
    {
        Log::error(header(), "portSwitchingMutex is null in handlePortSwitch!");
        return;
    }
    if (!m_sendMutex || !m_sendMutex.get())
    {
        Log::error(header(), "sendMutex is null in handlePortSwitch!");
        return;
    }
    if (!m_receiveMutex || !m_receiveMutex.get())
    {
        Log::error(header(), "receiveMutex is null in handlePortSwitch!");
        return;
    }

    try
    {
        uint16_t port = 0;
        bool port_valid = false;

        const int max_retries = 50;
        int retry_count = 0;

        while (!m_stopped->load() && retry_count < max_retries && !port_valid)
        {
            try
            {
                port = getClientAPI()->getBoostTCPServerPortNumber();

                if (port != 0xFF && port != 0)
                {
                    if (port < 1024)
                    {
                        Log::warning(header(), "Got system port number: ", port);
                    }
                    port_valid = true;
                    break;
                }
            }
            catch (const std::exception& e)
            {
                Log::error(header(), "Error getting port from ClientAPI: ", e.what());
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            retry_count++;
        }

        if (!port_valid)
        {
            if (m_stopped->load())
            {
                Log::debug(header(), "Port switch aborted due to shutdown");
            }
            else if (retry_count >= max_retries)
            {
                Log::error(header(), "Timeout waiting for valid port number (retries: ", retry_count, ")");
            }
            else
            {
                Log::error(header(), "Invalid port number received: ", port);
            }
            updateProtocolStatus(ProtocolStatus::error);
            throw std::runtime_error("Failed to get valid port number");
        }

        std::lock_guard<std::mutex> portLock(*m_portSwitchingMutex);
        std::lock_guard<std::mutex> sendLock(*m_sendMutex);
        std::lock_guard<std::mutex> recvLock(*m_receiveMutex);

        updateProtocolStatus(ProtocolStatus::switching_ports);
        Log::debug(header(), "Starting port switch to: ", port);

        std::shared_ptr<boost::asio::ip::tcp::socket> newSocket;
        try
        {
            newSocket = std::make_shared<boost::asio::ip::tcp::socket>(*m_ioContext);

            auto endpoints = m_resolver.resolve(
                    getClientAPI()->getBoostTCPServerAddress(),
                    std::to_string(port));

            if (endpoints.empty())
            {
                throw std::runtime_error("No endpoints resolved");
            }

            boost::system::error_code ec;
            boost::asio::connect(*newSocket, endpoints, ec);

            if (ec)
            {
                throw boost::system::system_error(ec);
            }
        }
        catch (const std::exception& e)
        {
            Log::error(header(), "Failed to create new socket connection: ", e.what());
            if (newSocket && newSocket->is_open())
            {
                boost::system::error_code ec;
                ec = newSocket->close(ec);
                if (ec)
                {
                    Log::error(header(), "Failed to close socket");
                }
            }
            throw;
        }

        if (!newSocket->is_open())
        {
            throw std::runtime_error("New socket failed to open");
        }

        auto oldSocket = exchangeSocket(newSocket);

        if (oldSocket)
        {
            boost::system::error_code shutdown_ec;
            boost::system::error_code close_ec;

            shutdown_ec = oldSocket->shutdown(boost::asio::ip::tcp::socket::shutdown_both, shutdown_ec);
            if (shutdown_ec)
            {
                // Socket might already be closed.
                if (shutdown_ec != boost::asio::error::not_connected &&
                    shutdown_ec != boost::asio::error::bad_descriptor)
                {
                    Log::warning(header(), "Socket shutdown warning: ", shutdown_ec.message());
                }
                else
                {
                    Log::debug(header(), "Socket already disconnected");
                }
            }

            close_ec = oldSocket->close(close_ec);
            if (close_ec)
            {
                // Only log if this isn't a "already closed" error.
                if (close_ec != boost::asio::error::bad_descriptor)
                {
                    Log::error(header(), "Socket close error: ", close_ec.message());
                }
            }
        }

        m_serverPort = port;
        updateProtocolStatus(ProtocolStatus::connected);

        if (!m_socket.load() || !m_socket.load()->is_open())
        {
            throw std::runtime_error("Socket verification failed after switch");
        }

        Log::info(header(), "Successfully switched to port ", m_serverPort);

        if (!m_stopped->load())
        {
            Log::debug(header(), "Restaring async read after port switch");
            boost::asio::post(*m_ioContext, [this]()
                              { startAsyncRead(); });
        }
    }

    catch (const std::exception& e)
    {
        Log::error(header(), "Port switch failed: ", e.what());
        updateProtocolStatus(ProtocolStatus::error);
        return;
    }
    catch (...)
    {
        Log::error(header(), "Unknown error during port switch");
        updateProtocolStatus(ProtocolStatus::error);
        return;
    }
}

void TCPClient::start()
{
    if (connectToServer())
    {
        startAsyncRead();

        // clang-format off
        m_ioContextThread = std::thread([this]()
        {
            Log::debug(header(), "io_context thread started.");
            try
            {
                m_ioContext->run();
            }
            catch (const std::exception& e)
            {
                Log::error(header(), "io_context exception: ", e.what());
            }
            Log::debug(header(), "io_context thread stopped.");
        });

        m_portSwitchingThread = std::thread([this]()
        {
            Log::debug(header(), "Port switching thread started");
            int wait_count = 20; // Two seconds
            while (!m_stopped->load() && wait_count > 0)
            {
                if (getProtocolStatus() == ProtocolStatus::connected)
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(50));
                    handlePortSwitch();
                    break;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                wait_count--;
            }

            if (wait_count < 0 && !m_stopped->load())
            {
                Log::warning(header(), "Port switching timed out waiting for connected state");
            }
            Log::debug(header(), "Port switching thread stopped");
        });

        // clang-format on
        ClientProtocol::start(getType());
    }
    else
    {
        Log::error(header(), "Failed to connect to the server");
    }
}

void TCPClient::startAsyncRead()
{
    if (m_stopped->load())
        return;

    auto current_socket = m_socket.load();

    if (!current_socket || !current_socket->is_open())
    {
        Log::warning(header(), "Socket not available for async read");
        return;
    }

    auto receiveBuffer = std::make_shared<boost::asio::streambuf>();
    Log::debug(header(), "Starting async read");

    boost::asio::async_read_until(*current_socket, *receiveBuffer, '\n',
                                  boost::asio::bind_executor(*m_strand, [this, receiveBuffer, current_socket](const boost::system::error_code& ec, size_t bytes_transferred)
                                                             {
                                                                 if (ec)
                                                                 {
                                                                     handleAsyncReadError(ec);
                                                                     return;
                                                                 }

                                                                 if (m_stopped->load())
                                                                 {
                                                                     Log::debug(header(), "Read completed but client stopped");
                                                                     return;
                                                                 }

                                                                 Log::debug(header(), "Read completed: ", bytes_transferred, " bytes");

                                                                 if (bytes_transferred > 0)
                                                                 {
                                                                     // Process the message.
                                                                     nx_data buffer(bytes_transferred);
                                                                     std::istream is(receiveBuffer.get());
                                                                     is.read(reinterpret_cast<char*>(buffer.data()), bytes_transferred);

                                                                     try
                                                                     {
                                                                         auto result = ClientProtocol::getClientAPI()->readMessage(buffer);
                                                                         if (result == ClientAPI::ReadResult::success)
                                                                         {
                                                                             Log::info(header(), "Message read successfully");
                                                                         }
                                                                     }
                                                                     catch (const std::exception& e)
                                                                     {
                                                                         Log::error(header(), "Error processing message: ", e.what());
                                                                     }
                                                                 }

                                                                 // Start the next async read.
                                                                  if (current_socket->is_open() && !m_stopped->load())
                                                                 {
                                                                     startAsyncRead();
                                                                 } }));
}

void TCPClient::handleAsyncReadError(const boost::system::error_code& ec)
{
    if (ec == boost::asio::error::eof)
    {
        Log::info(header(), "Connection closed by server (EOF) - this is normal");
    }
    else if (ec == boost::asio::error::connection_reset)
    {
        Log::info(header(), "Connection reset by peer");
    }
    else if (ec == boost::asio::error::operation_aborted)
    {
        Log::debug(header(), "Async read aborted (normal during shutdown)");
        return;
    }
    else if (ec == boost::asio::error::not_connected)
    {
        Log::warning(header(), "Socket not connected");
    }
    else
    {
        Log::error(header(), "Async read error: ", ec.message(), " (", ec.value(), ")");
    }

    updateProtocolStatus(ProtocolStatus::error);
}

std::shared_ptr<boost::asio::ip::tcp::socket> TCPClient::loadSocket() const
{
    auto boost_ptr = m_socket.load();
    if (boost_ptr)
    {
        return std::shared_ptr<boost::asio::ip::tcp::socket>(boost_ptr.get(),
                                                             [boost_ptr](auto&&...) mutable
                                                             {
                                                                 boost_ptr.reset();
                                                             });
    }
    return nullptr;
}

void TCPClient::storeSocket(std::shared_ptr<boost::asio::ip::tcp::socket> socket)
{
    if (socket)
    {
        m_socket.store(boost::shared_ptr<boost::asio::ip::tcp::socket>(
                socket.get(),
                [socket](auto&&...) mutable
                {
                    socket.reset();
                }));
    }
    else
    {
        m_socket.store(nullptr);
    }
}

std::shared_ptr<boost::asio::ip::tcp::socket> TCPClient::exchangeSocket(
        std::shared_ptr<boost::asio::ip::tcp::socket> new_socket)
{
    boost::shared_ptr<boost::asio::ip::tcp::socket> new_boost_ptr;
    if (new_socket)
    {
        new_boost_ptr = boost::shared_ptr<boost::asio::ip::tcp::socket>(
                new_socket.get(),
                [new_socket](auto&&...) mutable
                {
                    new_socket.reset();
                });
    }

    boost::shared_ptr<boost::asio::ip::tcp::socket> old_boost_ptr = m_socket.exchange(new_boost_ptr);

    if (old_boost_ptr)
    {
        return std::shared_ptr<boost::asio::ip::tcp::socket>(
                old_boost_ptr.get(),
                [old_boost_ptr](auto&&...) mutable
                {
                    old_boost_ptr.reset();
                });
    }

    return nullptr;
}

} // namespace nexilis::client::nxboost
