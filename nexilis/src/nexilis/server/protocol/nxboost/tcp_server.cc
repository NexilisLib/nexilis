#include <nexilis/ports.hh>
#include <nexilis/server/command.hh>
#include <nexilis/server/protocol/nxboost/tcp_server.hh>
#include <nexilis/util.hh>

#include <boost/asio/buffer.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/asio/write.hpp>

namespace nexilis::server::nxboost
{

TCPServer::TCPServer(const Settings& settings) noexcept
    : ServerProtocol(settings),
      NxClass("server::nxboost::TCPServer"),
      m_stopped(std::make_unique<std::atomic<bool>>(false)),
      m_mutex(std::make_unique<std::mutex>()),
      m_ioContext(std::make_unique<boost::asio::io_context>()),
      m_acceptor(*m_ioContext, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), Ports::getBoostTCPPort()))
{
    // Enable SO_REUSEADDR to allow port reuse.
    m_acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
}

TCPServer::TCPServer(TCPServer&& other) noexcept
    : Protocol(std::move(other)),
      ServerProtocol(std::move(other)),
      NxClass(std::move(other)),
      m_stopped(std::move(other.m_stopped) ? std::move(other.m_stopped) : std::make_unique<std::atomic<bool>>(false)),
      m_mutex(std::move(other.m_mutex)),
      m_ioContext(std::move(other.m_ioContext)),
      m_acceptor(std::move(other.m_acceptor)),
      m_listenThread(std::move(other.m_listenThread)),
      m_ioContextThread(std::move(other.m_ioContextThread)),
      m_serverPort(std::move(other.m_serverPort))
{
}

TCPServer& TCPServer::operator=(TCPServer&& other) noexcept
{
    if (this != &other)
    {
        m_stopped = std::move(other.m_stopped);
        if (!m_stopped)
        {
            m_stopped = std::make_unique<std::atomic<bool>>(false);
        }
        m_mutex = std::move(other.m_mutex);
        m_ioContext = std::move(other.m_ioContext);
        m_acceptor = std::move(other.m_acceptor);
        m_listenThread = std::move(other.m_listenThread);
        m_ioContextThread = std::move(other.m_ioContextThread);
        m_serverPort = std::move(other.m_serverPort);

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
    if (m_acceptor.cancel(ec))
    {
        Log::error("Error cancelling socket operations: ", ec.message());
    }
    if (ec)
    {
        Log::error("Cancel error: ", ec.message());
    }

    if (m_ioContext)
    {
        Log::debug("TCPServer stopping io_context");
        m_ioContext->stop();
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
        Log::debug("TCPServer closing ioContextThread");
        m_ioContextThread.join();
    }
    if (m_listenThread.joinable())
    {
        Log::debug("TCPServer closing listenThread");
        m_listenThread.join();
    }

    if (m_acceptor.close(ec))
    {
        Log::error("Error closing socket: ", ec.message());
    }
    if (ec)
    {
        Log::error("Closing error: ", ec.message());
    }
    Log::debug("BoostTCPServer stopped.");
}

bool TCPServer::startListening()
{
    m_acceptor.listen();
    m_serverPort = m_acceptor.local_endpoint().port();
    Log::debug("Boost TCP server started on port: ", m_serverPort);
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

void TCPServer::handleHandshake(boost::asio::ip::tcp::socket socket)
{
    auto thread = std::thread([this, hsSocket = std::move(socket)]() mutable
                              {
        try
        {
            boost::asio::streambuf buf;
            boost::asio::read(hsSocket, buf, boost::asio::transfer_at_least(1));
        }
        catch (...)
        {
            Log::debug(header(), "Problem with handshake");
        } });
}

void TCPServer::handleClient(boost::asio::ip::tcp::socket socket)
{
    // clang-format off
    auto client_thread = std::thread([this, newSocket = std::move(socket)]() mutable
    {
    // clang-format on
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

                if (type == BaseMessage::Type::auth_message)
                {
                    // First message, switch the port.
                    uint16_t newPort = switchToRandomPort();
                }
                else if (type == BaseMessage::Type::message)
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
                        Log::info(header(), "Result: ", Command::resultTypeAsString(passCommand));
                    }
                }
                else
                {
                    Log::critical(header(), "Unrecognized message type");
                }
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
    ServerProtocol::connectionClosed(); });

    {
        std::lock_guard<std::mutex> lock(*m_mutex);
        m_clientThreads.emplace_back(std::move(client_thread));
    }
}

bool TCPServer::acceptClients()
{
    // TODO switch
    bool handshakeComplete = true;

    try
    {
        while (!m_stopped->load())
        {
            // Create a new socket for each client connection
            boost::asio::ip::tcp::socket newSocket(*m_ioContext);
            boost::system::error_code ec;

            m_acceptor.non_blocking(true);

            // Try to accept with timeout.
            auto start = std::chrono::steady_clock::now();
            bool accepted = false;

            while (!m_stopped->load())
            {
                ec = m_acceptor.accept(newSocket, ec);

                if (!ec)
                {
                    // Successfully accepted a connection
                    auto remote_endpoint = newSocket.remote_endpoint();
                    Log::debug(header(), "Accepted connection from: ", remote_endpoint.address().to_string(), ":", remote_endpoint.port());
                    accepted = true;
                    break;
                }

                if (ec != boost::asio::error::would_block)
                {
                    // Real error occurred
                    if (ec != boost::asio::error::operation_aborted)
                    {
                        Log::error(header(), "Accept error: ", ec.message());
                    }
                    return false;
                }

                // Check timeout (100ms max wait)
                if (std::chrono::steady_clock::now() - start > std::chrono::milliseconds(100))
                {
                    break;
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }

            if (m_stopped->load())
            {
                Log::debug(header(), "m_stopped->load() called");
                break;
            }

            if (accepted)
            {
                if (!handshakeComplete)
                {
                    Log::debug("Unimplemented");
                }
                else
                {
                    handleClient(std::move(newSocket));
                }
            }
        }
    }
    catch (const std::exception& e)
    {
        Log::error("Exception in acceptClients: ", e.what());
        return false;
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
    // Close the current acceptor (fixed port).
    boost::system::error_code ec;
    if (!m_acceptor.close(ec))
    {
        Log::error("Failed to close acceptor: ", ec.message());
        return 0;
    }
    if (ec)
    {
        Log::error("Failed to close acceptor: ", ec.message());
        return 0;
    }

    // Rebind to a random port.
    m_acceptor.open(boost::asio::ip::tcp::v4());
    m_acceptor.bind(boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 0));
    m_acceptor.listen();

    m_serverPort = m_acceptor.local_endpoint().port();
    Log::debug("Switched to a random port: ", m_serverPort);
    return m_serverPort;
}

} // namespace nexilis::server::nxboost
