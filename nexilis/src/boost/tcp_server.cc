#include <nexilis/boost/tcp_server.hh>

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

bool TCPServer::acceptClient()
{
    m_acceptor.accept(m_socket);
    return m_socket.is_open();
}

bool TCPServer::sendToClient(const std::string& data) 
{
    ::boost::asio::write(m_socket, ::boost::asio::buffer(data));
    return true;
}

bool TCPServer::receiveFromClient(std::string& buffer)
{
    ::boost::asio::streambuf receiveBuffer;
    ::boost::asio::read_until(m_socket, receiveBuffer, '\n');
    buffer = ::boost::asio::buffer_cast<const char*>(receiveBuffer.data());
    return true;
}

}