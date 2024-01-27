#include <nexilis/boost/tcp_server.hh>

#include <nexilis/log.hh>

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
                    // Receive data from the client
                    ::boost::asio::streambuf receiveBuffer;
                    ::boost::system::error_code error;

                    size_t bytesRead = ::boost::asio::read(newSocket, receiveBuffer, ::boost::asio::transfer_at_least(1), error);

                    if (error == ::boost::asio::error::eof)
                    {
                        // Client closed the connection
                        break;
                    }
                    else if (error)
                    {
                        // Handle other errors
                        std::cerr << "Error reading from client: " << error.message() << std::endl;
                        break;
                    }

                    std::string message = ::boost::asio::buffer_cast<const char*>(receiveBuffer.data());

                    // Process the received message (replace with your logic)
                    std::cout << "Received from client: " << message << std::endl;

                    // Send a response back to the client
                    ::boost::asio::write(newSocket, ::boost::asio::buffer("Server received: " + message));
                }
            }
            catch (const ::boost::system::system_error& e)
            {
                // Handle errors or client disconnect here
                std::cerr << "Error in client thread: " << e.what() << std::endl;
            }
        }).detach(); // Detach the thread to run independently
    }
}

bool TCPServer::sendToClient(const std::string& data) 
{
    ::boost::asio::write(m_socket, ::boost::asio::buffer(data));
    return true;
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

}