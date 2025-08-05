#include <nexilis/client/protocol/nxboost/tcp_client.hh>
#include <nexilis/logger/log.hh>
#include <nexilis/ports.hh>

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
      m_workGuard(std::make_unique<boost::asio::executor_work_guard<boost::asio::io_context::executor_type>>(
              boost::asio::make_work_guard(*m_ioContext))),
      m_socket(),
      m_resolver(*m_ioContext),
      m_sendMutex(std::make_shared<std::mutex>()),
      m_receiveMutex(std::make_shared<std::mutex>()),
      m_portSwitchingMutex(std::make_shared<std::mutex>())
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
      m_receiveThread(std::move(other.m_receiveThread)),
      m_portSwitchingThread(std::move(other.m_portSwitchingThread)),
      m_stopped(std::move(other.m_stopped)),
      m_ioContext(std::move(other.m_ioContext)),
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
        if (m_receiveThread.joinable())
        {
            m_receiveThread.join();
        }
        if (m_portSwitchingThread.joinable())
        {
            m_portSwitchingThread.join();
        }

        m_ioContextThread = std::move(other.m_ioContextThread);
        m_receiveThread = std::move(other.m_receiveThread);
        m_portSwitchingThread = std::move(other.m_portSwitchingThread);
        m_stopped = std::move(other.m_stopped);
        m_ioContext = std::move(other.m_ioContext);
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

    if (m_receiveThread.joinable())
    {
        m_receiveThread.join();
        Log::debug(header(), "receivethread stopped");
    }

    if (m_ioContextThread.joinable())
    {
        m_ioContextThread.join();
        Log::debug(header(), "io_contextThread stopped");
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
    std::shared_ptr<boost::asio::ip::tcp::socket> current_socket = loadSocket();
    {
        std::lock_guard<std::mutex> portLock(*m_portSwitchingMutex);
        std::lock_guard<std::mutex> sockLock(*m_sendMutex);

        if (getProtocolStatus() != ProtocolStatus::connected ||
            m_stopped->load())
        {
            Log::warning(header(), "Cannot send in state: ", getProtocolStatusString());
            return false; // retry
        }
    }

    if (!current_socket || !current_socket->is_open())
    {
        Log::warning(header(), "Socket is not open during send");
        return false;
    }

    try
    {
        // clang-format off
        boost::asio::async_write(*current_socket, boost::asio::buffer(data),
            [this, current_socket](const boost::system::error_code& ec, std::size_t size)
            {
                if (!ec)
                {
                    Log::info(header(), "Message sent successfully with size of: ", size, " bytes");
                }
                else if (current_socket->is_open())
                {
                    boost::system::error_code ignore_ec;
                    ignore_ec = current_socket->close(ignore_ec);
                    if (ignore_ec)
                    {
                        Log::error(header(), "Error closing socket");
                    }
                }
        });
        // clang-format on
        return true;
    }
    catch (...)
    {
        Log::error(header(), "Exception during async_write");
        return false;
    }
}

void TCPClient::handlePortSwitch()
{
    try
    {
        uint16_t port = 0;
        const auto start = std::chrono::steady_clock::now();
        while (!m_stopped->load() && std::chrono::steady_clock::now() - start < std::chrono::seconds(5))
        {
            port = getClientAPI()->getBoostTCPServerPortNumber();
            if (port != 0xFF)
            {
                break;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        if (port == 0xFF)
        {
            throw std::runtime_error("Timeout waiting for valid port number");
        }

        std::lock_guard<std::mutex> portLock(*m_portSwitchingMutex);
        std::lock_guard<std::mutex> sendLock(*m_sendMutex);
        std::lock_guard<std::mutex> recvLock(*m_receiveMutex);

        updateProtocolStatus(ProtocolStatus::switching_ports);
        Log::debug(header(), "Starting port switch to: ", port);

        // Create new socket.
        auto newSocket = std::make_shared<boost::asio::ip::tcp::socket>(*m_ioContext);
        auto endpoints = m_resolver.resolve(
                getClientAPI()->getBoostTCPServerAddress(),
                std::to_string(port));

        boost::system::error_code ec;
        boost::asio::connect(*newSocket, endpoints, ec);
        if (ec)
        {
            throw boost::system::error_code(ec);
        }

        auto oldSocket = exchangeSocket(newSocket);

        if (oldSocket && oldSocket->is_open())
        {
            boost::system::error_code conn_ec;
            ec = oldSocket->shutdown(boost::asio::ip::tcp::socket::shutdown_both, conn_ec);
            if (conn_ec)
            {
                Log::error(header(), "Error shutting down socket");
            }
            conn_ec = oldSocket->close(conn_ec);
            if (conn_ec)
            {
                Log::error(header(), "Error closing socket");
            }
        }

        m_serverPort = port;
        updateProtocolStatus(ProtocolStatus::connected);

        if (!m_socket.load()->is_open())
        {
            throw std::runtime_error("Socket failed to open");
        }

        Log::info(header(), "Successfully switched to port ", m_serverPort);
    }

    catch (const std::exception& e)
    {
        Log::error(header(), "Port switch failed: ", e.what());
        return;
    }
    catch (...)
    {
        Log::error(header(), "Unknown error during port switch");
        return;
    }
}

void TCPClient::receive(const std::function<void(nx_data)>& callback)
{
    if (m_stopped->load())
    {
        Log::debug(header(), "stopped, cannot receive");
        return;
    }

    try
    {
        std::unique_lock<std::mutex> lock(*m_receiveMutex);
        auto receiveBuffer = std::make_shared<boost::asio::streambuf>();

        auto current_socket = m_socket.load();
        if (!current_socket)
        {
            Log::error(header(), "Socket is null in receive()");
            return;
        }

        // clang-format off
        boost::asio::async_read_until(*current_socket, *receiveBuffer, '\n',
            [this, receiveBuffer, callback, current_socket](const boost::system::error_code& ec, size_t bytes_transferred)
            {
                if (ec || m_stopped->load())
                {
                    if (ec != boost::asio::error::operation_aborted)
                    {
                        current_socket->close();
                    }
                    return;
                }

                if (bytes_transferred == 0)
                {
                    return;
                }

                nx_data buffer(bytes_transferred);
                std::istream is(receiveBuffer.get());
                is.read(reinterpret_cast<char*>(buffer.data()), bytes_transferred);
                Log::info(header(), "Received message of size: ", bytes_transferred);

                if (callback)
                {
                    callback(buffer);
                }

                if (current_socket->is_open())
                {
                    receive(callback);
                }
        });
    }
    catch (...)
    {
        Log::error(header(), "Error receiving messages");
    }
    // clang-format on
}

void TCPClient::start()
{
    if (connectToServer())
    {
        // clang-format off
        m_ioContextThread = std::thread([this]()
        {
            Log::debug(header(), "io_context thread started.");
            m_ioContext->run();
            Log::debug(header(), "io_context thread stopped.");
        });

        m_receiveThread = std::thread([this]()
        {
            Log::debug(header(), "Receive loop started");
            receiveLoop();
            Log::debug(header(), "Receive loop stopped");
        });

        m_portSwitchingThread = std::thread([this]()
        {
            Log::debug(header(), "Port switching thread started");
            while (!m_stopped->load())
            {
                if (getProtocolStatus() == ProtocolStatus::connected)
                {
                    handlePortSwitch();
                    break;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
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

void TCPClient::receiveLoop()
{
    while (!m_stopped->load())
    {
        auto cb = [this](const nx_data& buffer)
        {
            if (m_stopped->load())
            {
                return;
            }
            try
            {
                auto result = ClientProtocol::getClientAPI()->readMessage(buffer);
                if (result == ClientAPI::ReadResult::success)
                {
                    Log::info(header(), "Message read successfully");
                }
                else
                {
                    Log::warning(header(), "Received unexpected message: ");
                    Log::warning(header(), "Type: ", ClientAPI::readResultStr(result));
                    Util::debugUint8Vector(buffer);
                }
            }
            catch (const std::exception& e)
            {
                Log::error(header(), "Exception in receive callback: ", e.what());
            }
            catch (...)
            {
                Log::error(header(), "Unknown error in receive callback");
            }
        };

        receive(cb);
    }
}

std::shared_ptr<boost::asio::ip::tcp::socket> TCPClient::loadSocket() const
{
    auto boost_ptr = m_socket.load();
    if (boost_ptr)
    {
        return std::shared_ptr<boost::asio::ip::tcp::socket>(boost_ptr.get(),
                                                             [boost_ptr](boost::asio::ip::tcp::socket*) mutable
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
                [socket](boost::asio::ip::tcp::socket*) mutable
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
                [new_socket](boost::asio::ip::tcp::socket*) mutable
                {
                    new_socket.reset();
                });
    }

    boost::shared_ptr<boost::asio::ip::tcp::socket> old_boost_ptr = m_socket.exchange(new_boost_ptr);

    if (old_boost_ptr)
    {
        return std::shared_ptr<boost::asio::ip::tcp::socket>(
                old_boost_ptr.get(),
                [old_boost_ptr](boost::asio::ip::tcp::socket*) mutable
                {
                    old_boost_ptr.reset();
                });
    }

    return nullptr;
}

} // namespace nexilis::client::nxboost
