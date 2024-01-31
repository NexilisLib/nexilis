#include <cstdint>
#include <nexilis/command.hh>
#include <nexilis/common/util.hh>
#include <nexilis/boost/tcp_server.hh>

#include <nexilis/log.hh>
#include <string>

namespace nexilis::boost
{

TCPServer::TCPServer(const std::string& serverPort) :
    m_acceptor(m_ioService,
    ::boost::asio::ip::tcp::endpoint(::boost::asio::ip::tcp::v4(), std::stoi(serverPort))),
    m_socket(m_ioService)
{
}

TCPServer::~TCPServer()
{
    m_socket.close();
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
        // Create a new socket for each client connection
        ::boost::asio::ip::tcp::socket newSocket(m_ioService);
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
                        std::cerr << "Error: " << e.what() << std::endl;
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

                    auto clientSender = [this](const std::vector<uint8_t>& bytes)
                    {
                        std::string toString = Util::convertToString(bytes);
                        sendToClient(toString);
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
                std::cerr << "Error in client thread: " << e.what() << std::endl;
            } })
            .detach();
    }
}

bool TCPServer::sendToClient(const std::string& data)
{
    ::boost::asio::write(m_socket, ::boost::asio::buffer(data));
    return true;
}

void TCPServer::start()
{
    if (startListening())
    {
        acceptClients();
    }
}

bool TCPServer::receiveFromClient(std::string& buffer)
{
    ::boost::asio::streambuf receiveBuffer;

    try
    {
        // Attempt to read data from the socket
        size_t bytesRead = ::boost::asio::read_until(m_socket, receiveBuffer, '\n');

        // If bytesRead is 0, the client has closed the connection
        if (bytesRead == 0)
        {
            Log::info("Connection closed by client");
            return false;
        }

        // Convert the received data to a string
        std::istream is(&receiveBuffer);
        std::getline(is, buffer);

        return true;
    }
    catch (const ::boost::system::system_error& e)
    {
        // Handle boost::asio errors
        std::cerr << "Error receiving data: " << e.what() << std::endl;
        return false;
    }
    catch (const std::exception& e)
    {
        // Handle other exceptions
        std::cerr << "Exception: " << e.what() << std::endl;
        return false;
    }
}

} // namespace nexilis::boost
