#include <nexilis/server/command.hh>
#include <nexilis/server/message/auth_message.hh>
#include <nexilis/server/protocol/nxboost/tcp_server.hh>
#include <nexilis/util.hh>

#include <boost/asio/buffer.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/asio/write.hpp>

namespace nexilis::server::nxboost
{

TCPServer::TCPServer(const Settings& settings) noexcept
    : ServerProtocol(settings),
      NxClass("server::nxboost::TCPServer"),
      m_firstClientConnected(std::make_unique<std::atomic<bool>>(false)),
      m_stopped(std::make_unique<std::atomic<bool>>(false)),
      m_mutex(std::make_unique<std::mutex>()),
      m_ioContext(std::make_unique<boost::asio::io_context>()),
      m_acceptor(*m_ioContext, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), m_defaultServerPort)),
      m_switchedAcceptor(*m_ioContext),
      m_switchedPort(std::make_unique<std::atomic<uint16_t>>(0))
{
    // Enable SO_REUSEADDR to allow port reuse.
    m_acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    // m_switchedAcceptor will be configured later in switchToRandomPort().
}

TCPServer::TCPServer(TCPServer&& other) noexcept
    : Protocol(std::move(other)),
      ServerProtocol(std::move(other)),
      NxClass(std::move(other)),
      m_firstClientConnected(std::move(other.m_firstClientConnected) ? std::move(other.m_firstClientConnected) : std::make_unique<std::atomic<bool>>(false)),
      m_stopped(std::move(other.m_stopped) ? std::move(other.m_stopped) : std::make_unique<std::atomic<bool>>(false)),
      m_mutex(std::move(other.m_mutex)),
      m_ioContext(std::move(other.m_ioContext)),
      m_acceptor(std::move(other.m_acceptor)),
      m_switchedAcceptor(std::move(other.m_switchedAcceptor)),
      m_switchedPort(std::move(other.m_switchedPort) ? std::move(other.m_switchedPort) : std::make_unique<std::atomic<uint16_t>>(0)),
      m_listenThread(std::move(other.m_listenThread)),
      m_ioContextThread(std::move(other.m_ioContextThread)),
      m_switchedAcceptThread(std::move(other.m_switchedAcceptThread)),
      m_clientThreads(std::move(other.m_clientThreads))
{
}

TCPServer& TCPServer::operator=(TCPServer&& other) noexcept
{
    if (this != &other)
    {
        m_firstClientConnected = std::move(other.m_firstClientConnected);
        if (!m_firstClientConnected)
        {
            m_firstClientConnected = std::make_unique<std::atomic<bool>>(false);
        }
        m_stopped = std::move(other.m_stopped);
        if (!m_stopped)
        {
            m_stopped = std::make_unique<std::atomic<bool>>(false);
        }
        m_mutex = std::move(other.m_mutex);
        m_ioContext = std::move(other.m_ioContext);
        m_acceptor = std::move(other.m_acceptor);
        m_switchedAcceptor = std::move(other.m_switchedAcceptor);
        m_switchedPort = std::move(other.m_switchedPort);
        m_listenThread = std::move(other.m_listenThread);
        m_ioContextThread = std::move(other.m_ioContextThread);
        m_switchedAcceptThread = std::move(other.m_switchedAcceptThread);
        m_clientThreads = std::move(other.m_clientThreads);

        Protocol::operator=(std::move(other));
        ServerProtocol::operator=(std::move(other));
        NxClass::operator=(std::move(other));
    }
    return *this;
}

TCPServer::~TCPServer()
{
    stop();
}

void TCPServer::start()
{
    // clang-format off
    m_ioContextThread = std::thread([this]()
    {
        m_ioContext->run();
    });

    m_listenThread = std::thread([this]()
    {
        if (startListening())
        {
            acceptClients();
        }
    });
    // clang-format on
}

void TCPServer::stop()
{
    if (!m_stopped || m_stopped->exchange(true))
    {
        Log::debug("TCPServer stop already in progress or completed.");
        return;
    }

    boost::system::error_code ec;

    // Cancel acceptors.
    if (m_acceptor.cancel(ec))
    {
        Log::error(header(), "Error cancelling m_acceptor: ", ec.message());
    }
    if (m_switchedAcceptor.cancel(ec))
    {
        Log::error(header(), "Error cancelling m_switchedAcceptor: ", ec.message());
    }

    // Close sockets.
    if (m_acceptor.close(ec))
    {
        Log::error(header(), "Error closing m_acceptor: ", ec.message());
    }
    if (m_switchedAcceptor.close(ec))
    {
        Log::error(header(), "Error closing m_switchedAcceptor: ", ec.message());
    }

    // Stop io context.
    if (m_ioContext)
    {
        Log::debug("TCPServer stopping io_context");
        m_ioContext->stop();
    }

    // Join threads.
    if (m_switchedAcceptThread.joinable())
    {
        Log::debug(header(), "closing switchAcceptThread");
        m_switchedAcceptThread.join();
        Log::debug(header(), "switchAcceptThread finished");
    }

    if (m_listenThread.joinable())
    {
        Log::debug(header(), "closing listenThread");
        m_listenThread.join();
        Log::debug(header(), "listenThread finished");
    }

    {
        std::lock_guard<std::mutex> lock(*m_mutex);
        Log::debug("BoostTCPServer closing ", m_clientThreads.size(), " client connections...");
        for (auto& thread : m_clientThreads)
        {
            if (thread.joinable())
            {
                thread.join();
            }
        }
        m_clientThreads.clear();
    }

    if (m_ioContextThread.joinable())
    {
        Log::debug(header(), "closing ioContextThread");
        m_ioContextThread.join();
        Log::debug(header(), "ioContextThread finished");
    }

    Log::debug("BoostTCPServer stopped.");
}

void TCPServer::startSwitchedAccepting()
{
    m_switchedAcceptThread = std::thread([this]()
                                         {
        while (!m_stopped->load()) {
            try {
                boost::asio::ip::tcp::socket socket(*m_ioContext);
                boost::system::error_code ec;

                if (!m_switchedAcceptor.is_open())
                {
                    if (!m_stopped->load())
                    {
                        Log::warning(header(), "Acceptor closed unexpectedly");
                    }
                    break;
                }

                // Same select-based approach for switched acceptor
                fd_set read_fds;
                FD_ZERO(&read_fds);
                FD_SET(m_switchedAcceptor.native_handle(), &read_fds);

                timeval timeout;
                timeout.tv_sec = 0;
                timeout.tv_usec = 100000; // 100ms timeout

                int result = select(m_switchedAcceptor.native_handle() + 1, &read_fds, nullptr, nullptr, &timeout);

                if (result > 0) {
                    m_switchedAcceptor.accept(socket, ec);

                    if (ec)
                    {
                        if (ec == boost::asio::error::bad_descriptor)
                        {
                            if (!m_stopped->load())
                            {
                                Log::warning(header(), "Accept on closed acceptor");
                            }
                            break;
                        }
                        else if (ec != boost::asio::error::operation_aborted)
                        {
                            Log::error(header(), "Accept error: ", ec.message());
                        }
                        continue;
                    }

                    if (socket.is_open()) {
                        Log::debug(header(), "Accepted switched connection");
                        handleClient(std::move(socket));
                    }
                }
                else if (result < 0 && errno != EINTR) {
                    Log::error(header(), "Switched select error: ", strerror(errno));
                }
            }
            catch (...) {
                if (!m_stopped->load()) {
                    Log::error(header(), "Error in switched accept");
                }
            }
        } });
}

bool TCPServer::startListening()
{
    m_acceptor.listen();
    Log::debug("Boost TCP server started on default port: ", m_defaultServerPort);
    return true;
}

nx_data TCPServer::receiveMessage(boost::asio::ip::tcp::socket& socket)
{
    nx_data data;

    boost::asio::streambuf receiveBuffer;
    boost::system::error_code error_code;

    boost::asio::read(socket, receiveBuffer, boost::asio::transfer_at_least(1), error_code);

    if (error_code == boost::asio::error::eof)
    {
        return data;
    }
    else if (error_code == boost::asio::error::operation_aborted)
    {
        Log::debug(header(), "receiveMessage aborted due to shutdown");
        return data;
    }
    else if (error_code)
    {
        // Handle other errors
        Log::error(header(), "Error reading from client: ", error_code.message());
        return data;
    }

    // Get the sequence of const buffers from the streambuf.
    const boost::asio::const_buffer& receive_buffer = receiveBuffer.data();

    // Check if the buffer is empty.
    if (receive_buffer.size() == 0)
    {
        Log::info(header(), "Empty data");
        return data;
    }

    // Extract the data.
    const uint8_t* buffer_data = static_cast<const uint8_t*>(receive_buffer.data());
    size_t buffer_size = receive_buffer.size();

    // Insert the data from the buffer into the vector.
    data.insert(data.end(), buffer_data, buffer_data + buffer_size);

    return data;
}

std::string TCPServer::getClientAddress(boost::asio::ip::tcp::socket& socket)
{
    std::string clientAddress;
    try
    {
        boost::asio::ip::tcp::endpoint remoteEndpoint = socket.remote_endpoint();
        boost::asio::ip::address remoteAddress = remoteEndpoint.address();
        clientAddress = remoteAddress.to_string();
    }
    catch (const std::exception& e)
    {
        Log::error("Error getting info from remote, reason: ", e.what());
    }
    return clientAddress;
}

void TCPServer::handleHandshake(boost::asio::ip::tcp::socket socket, std::function<void()> onCompleted)
{
    try
    {
        // clang-format off
        auto thread = std::thread([this, hs_socket = std::move(socket), cb = std::move(onCompleted)]() mutable
        {
            try
            {
                while (!m_stopped->load())
                {
                    if (!hs_socket.is_open())
                    {
                        break;
                    }

                    auto clientAddress = getClientAddress(hs_socket);
                    auto data = receiveMessage(hs_socket);
                    Log::debug(header(), "Received handshake message");

                    if (data.empty())
                    {
                        continue;
                    }

                    // TODO This is the passwd message
                    auto handled_message = getMessageHandler().readMessage(clientAddress, data, &getCommand().getSettings());

                    if (!handled_message)
                    {
                        continue;
                    }

                    // TODO fix this as well
                    handled_message->getUser()->setBoostTCPSend([this, &hs_socket](const nx_data& bytes)
                    {
                        if (sendToClient(bytes, hs_socket))
                        {
                            Log::info(header(), "Sent message to client succesfully");
                        }
                        else
                        {
                            Log::error(header(), "Error sending message to client");
                        }
                    });

                    auto type = handled_message->getType();

                    if (type == BaseMessage::Type::auth_message)
                    {
                        auto msgPtr = static_cast<AuthMessage*>(handled_message.get());

                        // TODO fix this also
                        Command::Result passCommand = getCommand().read(msgPtr->getData()[0], *msgPtr->getUser(), *this, msgPtr->getMessageId());

                        if (passCommand == Command::Result::success)
                        {
                            Log::info("Auth part 1 success");

                            if (!m_firstClientConnected->exchange(true))
                            {
                                Log::info(header(), "First client! Opening second port");
                                switchToRandomPort();
                            }

                            nx_data command_data = { 0, 1, 0, 1 };
                            auto port_data = Util::convertToByteVector(getPort());

                            for (auto&& data : port_data)
                            {
                                command_data.emplace_back(data);
                            }

                            Command::Result portCommand = getCommand().read(command_data, *msgPtr->getUser(), *this, msgPtr->getMessageId());

                            if (portCommand == Command::Result::success)
                            {
                                Log::info(header(), "Auth fully complete");
                                handled_message->getUser()->setBoostTCPSend(nullptr);
                                cb();
                                break;
                            }
                            else
                            {
                                Log::error("Auth failed to port data");
                            }
                        }
                        else
                        {
                            Log::info(header(), "Result: ", Command::resultTypeAsString(passCommand));
                        }
                    }
                    else
                    {
                        Log::critical(header(), "Wrong message type");
                    }
                }
            }
            catch (...)
            {
                Log::debug(header(), "Error with handshake");
            }

            boost::system::error_code ec;
            if (hs_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec))
            {
                Log::error(header(), "Error in client socket shutdown");
            }
            if (hs_socket.close(ec))
            {
                Log::error(header(), "Error in client socket close");
            }
        });

        {
            std::lock_guard<std::mutex> lock(*m_mutex);
            m_clientThreads.emplace_back(std::move(thread));
        }
    }
    catch (const std::exception& e)
    {
        Log::error(header(), "Failed to create handshake thread: ", e.what());
    }
    catch (...)
    {
        Log::error(header(), "Other failure in handshake thread");
    }
    // clang-format on
}

void TCPServer::handleClient(boost::asio::ip::tcp::socket socket)
{
    Log::warning(header(), "Handleclient called");

    // clang-format off
    auto client_thread = std::thread([this, newSocket = std::move(socket)]() mutable
    {
        ServerProtocol::connectionEstablished();
        try
        {
            std::string clientAddress = getClientAddress(newSocket);

            while (!m_stopped->load())
            {
                auto data = receiveMessage(newSocket);

                if (data.empty())
                {
                    if (!newSocket.is_open())
                    {
                        break;
                    }
                    continue;
                }

                auto handledMessage = getMessageHandler().readMessage(clientAddress, data, &getCommand().getSettings());

                if (!handledMessage)
                {
                    Log::error(header(), "Received invalid message");
                    continue;
                }

                if (!handledMessage->getUser()->isBoostTCPSet())
                {
                    handledMessage->getUser()->setBoostTCPSend([this, &newSocket](const nx_data& bytes)
                    {
                        if (sendToClient(bytes, newSocket))
                        {
                            Log::info(header(), "Sent message to client succesfully");
                        }
                        else
                        {
                            Log::error(header(), "Error sending message to client");
                        }
                    });
                }

                /// Get the type of the message.
                auto type = handledMessage->getType();

                if (type == BaseMessage::Type::message)
                {
                    auto msgPtr = static_cast<Message*>(handledMessage.get());

                    // Everything ok with initialization.
                    Command::Result passCommand = getCommand().read(msgPtr->getData(), *msgPtr->getUser(), *this, msgPtr->getMessageId());

                    if (passCommand == Command::Result::success)
                    {
                        Log::info(header(), "command success");
                    }
                    else
                    {
                        Log::warning(header(), "Result: ", Command::resultTypeAsString(passCommand));
                    }
                }
                else
                {
                    Log::critical(header(), "Wrong message type");
                }
            }
        }
        catch (const boost::system::system_error& e)
        {
            // Handle errors or client disconnect here
            Log::error(header(), "Error in client thread: ", e.what());
        }
        catch (const std::exception& e)
        {
            Log::error(header(), "Exception in client thread: ", e.what());
        }

        boost::system::error_code ec;
        if (newSocket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec))
        {
            Log::error(header(), "Error in client socket shutdown");
        }
        if (newSocket.close(ec))
        {
            Log::error(header(), "Error in client socket close");
        }
        ServerProtocol::connectionClosed(); });

    {
        std::lock_guard<std::mutex> lock(*m_mutex);
        m_clientThreads.emplace_back(std::move(client_thread));
    }
    // clang-format on
}

bool TCPServer::acceptClients()
{
    try
    {
        while (!m_stopped->load())
        {
            boost::asio::ip::tcp::socket socket(*m_ioContext);
            boost::system::error_code ec;

            if (!m_acceptor.is_open())
            {
                if (!m_stopped->load())
                {
                    Log::warning(header(), "Acceptor closed suddenly");
                }
                break;
            }

            // Use select() for interruptible accept
            fd_set read_fds;
            FD_ZERO(&read_fds);
            FD_SET(m_acceptor.native_handle(), &read_fds);

            timeval timeout;
            timeout.tv_sec = 0;
            timeout.tv_usec = 100000; // 100ms timeout

            int result = select(m_acceptor.native_handle() + 1, &read_fds, nullptr, nullptr, &timeout);

            if (result > 0)
            {
                m_acceptor.accept(socket, ec);

                if (ec)
                {
                    if (ec == boost::asio::error::bad_descriptor)
                    {
                        if (!m_stopped->load())
                        {
                            Log::warning(header(), "Accept on closed acceptor");
                        }
                        break;
                    }
                    else if (ec != boost::asio::error::operation_aborted)
                    {
                        Log::error(header(), "Accept error: ", ec.message());
                    }
                    continue;
                }

                if (socket.is_open())
                {
                    Log::debug(header(), "New handshake connection");
                    handleHandshake(std::move(socket), []()
                                    { Log::info("Handshake completed!"); });
                }
            }
            else if (result < 0 && errno != EINTR)
            {
                Log::error(header(), "Select error: ", strerror(errno));
            }

            if (m_stopped->load())
            {
                break;
            }
        }
    }
    catch (...)
    {
        if (!m_stopped->load())
        {
            Log::error(header(), "Exception in accept loop");
        }
    }
    return true;
}

bool TCPServer::sendToClient(const nx_data& data, boost::asio::ip::tcp::socket& clientSocket)
{
    if (clientSocket.is_open())
    {
        boost::system::error_code ec;
        boost::asio::write(clientSocket, boost::asio::buffer(data), ec);
        if (ec)
        {
            Log::error(header(), "Failed to send data: ", ec.message());
            return false;
        }
        return true;
    }
    return false;
}

uint16_t TCPServer::switchToRandomPort()
{
    boost::system::error_code ec;

    // Close the previous switched acceptor if any.
    if (m_switchedAcceptor.close(ec))
    {
        Log::error(header(), "Failed to close previous m_switchedAcceptor");
    }

    // Configure new switched port.
    if (m_switchedAcceptor.open(boost::asio::ip::tcp::v4(), ec))
    {
        Log::error(header(), "Failed to open switched acceptor: ", ec.message());
        return 0;
    }

    if (m_switchedAcceptor.bind(boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 0), ec))
    {
        Log::error(header(), "Failed to bind switched acceptor: ", ec.message());
        return 0;
    }

    if (m_switchedAcceptor.listen(boost::asio::socket_base::max_listen_connections, ec))
    {
        Log::error(header(), "Failed to listen on switched acceptor: ", ec.message());
        return 0;
    }

    m_switchedPort->store(m_switchedAcceptor.local_endpoint().port());

    startSwitchedAccepting();

    Log::debug(header(), "Now listening on default port: ", m_defaultServerPort, " and switched port: ", m_switchedPort->load());
    return m_switchedPort->load();
}

uint16_t TCPServer::getPort() const
{
    if (m_switchedPort && m_switchedPort.get() && m_switchedPort.get()->load())
    {
        return m_switchedPort.get()->load();
    }
    return 0;
}

} // namespace nexilis::server::nxboost
