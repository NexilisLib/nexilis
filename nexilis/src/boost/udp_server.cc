#include <nexilis/boost/udp_server.hh>
#include <nexilis/log.hh>

namespace nexilis::boost
{

UDPServer::UDPServer(int port) :
    m_ioContext(std::make_unique<::boost::asio::io_context>()),
    m_mutex(std::make_unique<std::mutex>()),
    m_socket(*m_ioContext, ::boost::asio::ip::udp::endpoint
    (::boost::asio::ip::udp::v4(), port)),
    m_receiveBuffer(NEXILIS_BUFFER)
{
}

UDPServer::~UDPServer()
{
    m_socket.close();
    m_ioContext->stop();

    if (m_ioContextThread.joinable())
    {
        m_ioContextThread.join();
    }
}

UDPServer::UDPServer(UDPServer&& other) :
    Protocol(std::move(other)),
    m_ioContext(std::move(other.m_ioContext)),
    m_mutex(std::move(other.m_mutex)),
    m_socket(std::move(other.m_socket)),
    m_remoteEndpoint(std::move(other.m_remoteEndpoint)),
    m_receiveBuffer(std::move(other.m_receiveBuffer)),
    m_ioContextThread(std::move(other.m_ioContextThread))
{
}

UDPServer& UDPServer::operator=(UDPServer&& other)
{
    if (this != &other)
    {
        Protocol::operator=(std::move(other));
        m_ioContext = std::move(other.m_ioContext);
        m_mutex = std::move(other.m_mutex);
        m_socket = std::move(other.m_socket);
        m_remoteEndpoint = std::move(other.m_remoteEndpoint);
        m_receiveBuffer = std::move(other.m_receiveBuffer);
        m_ioContextThread = std::move(other.m_ioContextThread);
    }
    return *this;
}

void UDPServer::start()
{
    m_ioContextThread = std::thread([this](){ m_ioContext->run(); });
    receiveFromClients();
}

void UDPServer::receiveFromClients()
{
    std::cout << "Receivefromclients called" << std::endl;

    m_remoteEndpoint = ::boost::asio::ip::udp::endpoint();

    m_socket.async_receive_from(
    ::boost::asio::buffer(m_receiveBuffer), m_remoteEndpoint,
        [this](const ::boost::system::error_code& error, std::size_t bytes_transferred)
        {
            if (!error)
            {
                /*
                {
                    std::lock_guard<std::mutex> lock(*m_mutex); // Lock the mutex
                    std::cout << "Received from " << m_remoteEndpoint.address().to_string() << ": "
                              << std::string(m_receiveBuffer.data(), bytes_transferred) << std::endl;
                } 
                */

                std::cout << "Received from " << m_remoteEndpoint.address().to_string() << ": "
                            << std::string(m_receiveBuffer.data(), bytes_transferred) << std::endl;
                // Continue listening for incoming messages from any endpoint
                receiveFromClients();
            }
            else
            {
                std::cerr << "Error receiving message: " << error.message() << std::endl;
                // Continue listening for incoming messages from any endpoint even after an error
                receiveFromClients();
            }
        }
    );
}

}
