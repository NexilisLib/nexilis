#include <nexilis/client/protocol/nxboost/tcp_client.hh>
#include <nexilis/command_type.hh>
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
      m_mainSocket(std::make_shared<boost::asio::ip::tcp::socket>(*m_ioContext)),
      m_switchedSocket(std::make_shared<boost::asio::ip::tcp::socket>(*m_ioContext)),
      m_activeSocket(),
      m_resolver(*m_ioContext),
      m_sendMutex(std::make_shared<std::mutex>()),
      m_receiveMutex(std::make_shared<std::mutex>()),
      m_portSwitchingMutex(std::make_shared<std::mutex>()),
      m_mainPort(Ports::getBoostTCPPort()),
      m_switchedPort(0),
      m_useSwitchedPort(false),
      m_pendingSendsMutex(std::make_shared<std::mutex>())
{
    storeSocket(m_mainSocket);

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
      m_stopped(std::move(other.m_stopped)),
      m_ioContext(std::move(other.m_ioContext)),
      m_strand(std::move(other.m_strand)),
      m_workGuard(std::move(other.m_workGuard)),
      m_mainSocket(std::move(other.m_mainSocket)),
      m_switchedSocket(std::move(other.m_switchedSocket)),
      m_activeSocket(other.m_activeSocket.load()),
      m_resolver(std::move(other.m_resolver)),
      m_sendMutex(std::move(other.m_sendMutex)),
      m_receiveMutex(std::move(other.m_receiveMutex)),
      m_portSwitchingMutex(std::move(other.m_portSwitchingMutex)),
      m_mainPort(std::move(other.m_mainPort)),
      m_switchedPort(std::move(other.m_switchedPort)),
      m_useSwitchedPort(other.m_useSwitchedPort.load()),
      m_pendingSends(std::move(other.m_pendingSends)),
      m_pendingSendsMutex(std::move(other.m_pendingSendsMutex))
{
    other.m_stopped.reset();
    other.m_strand.reset();
    other.m_workGuard.reset();
    other.m_sendMutex.reset();
    other.m_receiveMutex.reset();
    other.m_portSwitchingMutex.reset();
    other.m_activeSocket.store(nullptr);
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

        m_ioContextThread = std::move(other.m_ioContextThread);
        m_stopped = std::move(other.m_stopped);
        m_ioContext = std::move(other.m_ioContext);
        m_strand = std::move(other.m_strand);
        m_workGuard = std::move(other.m_workGuard);
        m_mainSocket = std::move(other.m_mainSocket);
        m_switchedSocket = std::move(other.m_switchedSocket);
        m_activeSocket = other.m_activeSocket.load();
        m_resolver = std::move(other.m_resolver);
        m_sendMutex = std::move(other.m_sendMutex);
        m_receiveMutex = std::move(other.m_receiveMutex);
        m_portSwitchingMutex = std::move(other.m_portSwitchingMutex);
        m_mainPort = std::move(other.m_mainPort);
        m_switchedPort = std::move(other.m_switchedPort);
        m_useSwitchedPort = other.m_useSwitchedPort.load();
        m_pendingSends = std::move(other.m_pendingSends);
        m_pendingSendsMutex = std::move(other.m_pendingSendsMutex);

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

bool TCPClient::connectToMainPort()
{
    try
    {
        Log::debug(header(), "Connect to main port: ", m_mainPort);

        auto endpoints = m_resolver.resolve(
                getClientAPI()->getBoostTCPServerAddress(),
                std::to_string(m_mainPort));

        boost::system::error_code ec;
        boost::asio::connect(*m_mainSocket, endpoints, ec);

        if (ec)
        {
            Log::error(header(), "Cannot connec to main port: ", m_mainPort, " - ", ec.message());
            return false;
        }
        Log::debug(header(), "Successfully connected to main port: ", m_mainPort);
        return true;
    }
    catch (const std::exception& e)
    {
        Log::error(header(), "Exceptino connecting to main port: ", e.what());
        return false;
    }
}

bool TCPClient::connectToSwitchedPort(uint16_t port)
{
    try
    {
        Log::debug(header(), "Connecting to switched port: ", port);

        auto endpoints = m_resolver.resolve(
                getClientAPI()->getBoostTCPServerAddress(),
                std::to_string(port));

        boost::system::error_code ec;
        boost::asio::connect(*m_switchedSocket, endpoints, ec);

        if (ec)
        {
            Log::error(header(), "Cannot connect to switched port: ", port, " - ", ec.message());
            return false;
        }

        m_switchedPort = port;
        Log::debug(header(), "Successfully connected to switched port: ", port);
        return true;
    }
    catch (const std::exception& e)
    {
        Log::error(header(), "Exception connecting to switched port: ", e.what());
        return false;
    }
}

void TCPClient::stop()
{
    if (m_stopped && m_stopped->exchange(true))
    {
        return;
    }

    // Close both sockets.
    if (m_mainSocket && m_mainSocket->is_open())
    {
        boost::system::error_code ec;
        ec = m_mainSocket->close(ec);
        if (ec)
        {
            Log::error(header(), "Error closing main socket");
        }
    }

    if (m_switchedSocket && m_switchedSocket->is_open())
    {
        boost::system::error_code ec;
        ec = m_switchedSocket->close(ec);
        if (ec)
        {
            Log::error(header(), "Error closing switched socket");
        }
    }

    m_workGuard.reset();

    if (m_ioContext)
    {
        m_ioContext->stop();
        Log::debug(header(), "io_context stopped");
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

    updateProtocolStatus(ProtocolStatus::connecting);

    // First connect to main port.
    if (!connectToMainPort())
    {
        updateProtocolStatus(ProtocolStatus::error);
        return false;
    }

    // Main socket is active for now.
    storeSocket(m_mainSocket);
    m_useSwitchedPort = false;

    updateProtocolStatus(ProtocolStatus::connected);
    return true;
}

void TCPClient::initiatePortSwitch(uint16_t port)
{
    Log::info(header(), "Initiating port switch to: ", port);

    boost::asio::post(*m_ioContext, [this, port]()
                      {
        Log::debug(header(), "Port switch task running, connecting to: ", port);
        if (connectToSwitchedPort(port))
        {
            Log::debug(header(), "Connected to switched port, updating active socket");
            storeSocket(m_switchedSocket);
            m_useSwitchedPort = true;

            if (m_mainSocket->is_open())
            {
                Log::debug(header(), "Closing main socket");
                boost::system::error_code ec;
                ec = m_mainSocket->close(ec);
                if (ec)
                {
                    Log::error(header(), "Error closing main socket");
                }
            }

            Log::info(header(), "Successfully switched to port: ", port);

            if (!m_stopped->load())
            {
                Log::debug(header(), "Starting async read on switched port");
                startAsyncRead();
            }
        }
        else
        {
            Log::error(header(), "Failed to connect to switched port: ", port);
        } });
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
        // clang-format off
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
        // clang-format on
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

    auto current_socket = loadSocket();

    if (!current_socket || !current_socket->is_open())
    {
        Log::warning(header(), "Socket not available for async read");
        return;
    }

    auto receiveBuffer = std::make_shared<boost::asio::streambuf>();
    Log::debug(header(), "Starting async read on ", (m_useSwitchedPort ? "switched" : "main"), " port");

    // clang-format off
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
                        Log::debug(header(), "readMessage result: ", ClientAPI::readResultStr(result));
                        if (result == ClientAPI::ReadResult::success)
                        {
                            Log::info(header(), "Message read successfully");

                            if (!m_useSwitchedPort)
                            {
                                uint16_t switchPort = getClientAPI()->getBoostTCPServerPortNumber();
                                Log::debug(header(), "Port switch check: useSwitched=", m_useSwitchedPort.load(),
                                           " switchPort=", switchPort, " mainPort=", m_mainPort);
                                if (switchPort != 0 && switchPort != 0xFF)
                                {
                                    Log::info(header(), "Port switch detected, switching from ", m_mainPort,
                                              " to ", switchPort);
                                    initiatePortSwitch(switchPort);
                                    return;
                                }
                                else
                                {
                                    Log::debug(header(), "No port switch needed (port=", switchPort, ")");
                                }
                            }
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
    // clang-format on
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
    auto boost_ptr = m_activeSocket.load();
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
        m_activeSocket.store(boost::shared_ptr<boost::asio::ip::tcp::socket>(
                socket.get(),
                [socket](auto&&...) mutable
                {
                    socket.reset();
                }));
    }
    else
    {
        m_activeSocket.store(nullptr);
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

    auto old_boost_ptr = m_activeSocket.exchange(new_boost_ptr);

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
