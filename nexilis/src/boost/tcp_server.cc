#include <nexilis/command.hh>
#include <nexilis/common/util.hh>
#include <nexilis/boost/tcp_server.hh>

#include <nexilis/log.hh>

namespace nexilis::boost
{

TCPServer::TCPServer(int serverPort) :
    m_mutex(std::make_unique<std::mutex>()),
    m_ioContext(std::make_unique<::boost::asio::io_context>()),
    m_acceptor(*m_ioContext,
    ::boost::asio::ip::tcp::endpoint(::boost::asio::ip::tcp::v4(), std::stoi(std::to_string(serverPort)))),
    m_socket(*m_ioContext)
{
}

TCPServer::TCPServer(TCPServer&& other) :
    Protocol(std::move(other)),
    m_mutex(std::move(other.m_mutex)),
    m_ioContext(std::move(other.m_ioContext)),
    m_acceptor(std::move(other.m_acceptor)),
    m_socket(std::move(other.m_socket)),
    m_listenThread(std::move(other.m_listenThread)),
    m_ioContextThread(std::move(other.m_ioContextThread))
{
}

TCPServer& TCPServer::operator=(TCPServer&& other)
{
    if (this != &other)
    {
        Protocol::operator=(std::move(other));
        m_mutex = std::move(other.m_mutex);
        m_ioContext = std::move(other.m_ioContext);
        m_acceptor = std::move(other.m_acceptor);
        m_socket = std::move(other.m_socket);
        m_listenThread = std::move(other.m_listenThread);
        m_ioContextThread = std::move(other.m_ioContextThread);
    }
    return *this;
}

TCPServer::~TCPServer()
{
    m_socket.close();
    stop();
}

void TCPServer::start()
{
    m_ioContextThread = std::thread([this]() { m_ioContext->run(); });
    
    m_listenThread = std::thread([this]()
    {
        if (startListening())
        {
            acceptClients();
        }
    });
}

void TCPServer::stop()
{
    m_ioContext->stop();

    if (m_ioContextThread.joinable())
    {
        m_ioContextThread.join();
    }
    if (m_listenThread.joinable())
    {
        m_listenThread.join();
    }
}

bool TCPServer::startListening()
{
    m_acceptor.listen();
    return true;
}

bool TCPServer::acceptClients()
{
    while (true)
    {
        std::lock_guard<std::mutex> lock(*m_mutex);

        // Create a new socket for each client connection
        ::boost::asio::ip::tcp::socket newSocket(*m_ioContext);
        m_acceptor.accept(newSocket);

        // Handle each client in a separate thread
        std::thread([this, newSocket = std::move(newSocket)]() mutable
        {
            try
            {
                while (true)
                {
                    std::string clientAddress;
                    uint16_t clientPort;
                    try
                    {
                        ::boost::asio::ip::tcp::endpoint remoteEndpoint = newSocket.remote_endpoint();
                        ::boost::asio::ip::address remoteAddress = remoteEndpoint.address();
                        clientAddress = remoteAddress.to_string();
                        clientPort = remoteEndpoint.port();
                        Log::debug("Remote IP address: ", clientAddress);
                    }
                    catch (const std::exception& e)
                    {
                        Log::error("TCPServer: Error getting info from remote, reason: ", e.what());
                    }

                    // Receive data from the client
                    ::boost::asio::streambuf receiveBuffer;
                    ::boost::system::error_code error;

                    size_t bytesRead = ::boost::asio::read(newSocket, receiveBuffer, ::boost::asio::transfer_at_least(1), error);

                    if (error == ::boost::asio::error::eof)
                    {
                        Log::debug("End receive ", clientAddress);
                        break;
                    }
                    else if (bytesRead <= 0)
                    {
                        // Other type of error.
                        Log::error("TCPServer Error: boost::asio::read");
                        break;
                    }
                    else if (error)
                    {
                        // Handle other errors
                        Log::error("TCPServer Error reading from client: ", error.message());
                        break;
                    }

                    std::string message = ::boost::asio::buffer_cast<const char*>(receiveBuffer.data());
                    Log::info("TCPServer Received from client: ", message);
                    auto handledMessage = getMessageHandler().readMessage(clientAddress, message, clientPort, Command::getAuthentication());

                    auto clientSender = [this, &newSocket](const std::vector<uint8_t>& bytes)
                    {
                        std::string toString = Util::convertToString(bytes);
                        if (sendToClient(toString, newSocket))
                        {
                            Log::info("Sended message to client succesfully");
                        }
                        else
                        {
                            Log::error("Error sending message to client");
                        }
                    };

                    bool passCommand = Command::read(handledMessage.getData(), *handledMessage.getClient(), *this, clientSender);

                    if (passCommand)
                    {
                        Log::info("Passed with command: ", message);
                    }
                    else
                    {
                        Log::info("Failed with command", message);
                    }
                }
            }
            catch (const ::boost::system::system_error& e)
            {
                // Handle errors or client disconnect here
                Log::error("Error in client thread: ", e.what());
            } })
            .detach();
    }
}

bool TCPServer::sendToClient(const std::string& data, ::boost::asio::ip::tcp::socket& clientSocket)
{
    Log::info("sendToClient called!");
    if (clientSocket.is_open())
    {
        ::boost::asio::write(clientSocket, ::boost::asio::buffer(data));
        return true;
    }
    return false;
}

} // namespace nexilis::boost
