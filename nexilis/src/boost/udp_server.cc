#include <boost/asio/ip/address.hpp>
#include <boost/system/system_error.hpp>
#include <nexilis/boost/udp_server.hh>
#include <nexilis/log.hh>
#include <nexilis/command.hh>

namespace nexilis::boost
{

UDPServer::UDPServer(int port)
    : m_ioContext(std::make_unique<boost::asio::io_context>()),
      m_mutex(std::make_unique<std::mutex>()),
      m_remoteEndpoint(boost::asio::ip::udp::v4(), port),
      m_socket(*m_ioContext, m_remoteEndpoint),
      m_receiveBuffer(NEXILIS_BUFFER)
{
    // Set socket option to allow address reuse
    boost::asio::ip::udp::socket::reuse_address reuse(true);
    m_socket.set_option(reuse);
}

UDPServer::~UDPServer()
{
    m_socket.close();
    m_ioContext->stop();

    if (m_ioContextThread.joinable())
    {
        m_ioContextThread.join();
    }

    if (m_receiveThread.joinable())
    {
        m_receiveThread.join();
    }
}

UDPServer::UDPServer(UDPServer&& other)
    : Protocol(std::move(other)),
      ServerProtocol(std::move(other)),
      m_ioContext(std::move(other.m_ioContext)),
      m_mutex(std::move(other.m_mutex)),
      m_remoteEndpoint(std::move(other.m_remoteEndpoint)),
      m_socket(std::move(other.m_socket)),
      m_receiveBuffer(std::move(other.m_receiveBuffer)),
      m_ioContextThread(std::move(other.m_ioContextThread)),
      m_receiveThread(std::move(other.m_ioContextThread))
{
}

UDPServer& UDPServer::operator=(UDPServer&& other)
{
    if (this != &other)
    {
        Protocol::operator=(std::move(other));
        ServerProtocol::operator=(std::move(other));
        m_ioContext = std::move(other.m_ioContext);
        m_mutex = std::move(other.m_mutex);
        m_remoteEndpoint = std::move(other.m_remoteEndpoint);
        m_socket = std::move(other.m_socket);
        m_receiveBuffer = std::move(other.m_receiveBuffer);
        m_ioContextThread = std::move(other.m_ioContextThread);
        m_receiveThread = std::move(other.m_receiveThread);
    }
    return *this;
}

void UDPServer::start()
{
    m_ioContextThread = std::thread([this]()
                                    { m_ioContext->run(); });

    m_receiveThread = std::thread(&UDPServer::receiveFromClients, this);
}

void UDPServer::receiveFromClients()
{
    std::lock_guard<std::mutex> lock(*m_mutex);

    while (m_socket.is_open())
    {
        try
        {
            std::size_t bytes_transferred = m_socket.receive_from(boost::asio::buffer(m_receiveBuffer), m_remoteEndpoint);

            std::string data = std::string(m_receiveBuffer.data(), bytes_transferred);
            std::string address = m_remoteEndpoint.address().to_string();
            uint16_t port = m_remoteEndpoint.port();
            Log::info("Received from ", address, " port ", port, " data: ", data);

            auto handledMessage = getMessageHandler().readMessage(address, data, port, Command::getAuthentication());

            if (!handledMessage.getClient()->isBoostUDPSet())
            {
                handledMessage.getClient()->setBoostUDPSend([this](const std::vector<uint8_t>& bytes)
                {
                    std::string byteString = Util::convertToString(bytes);
                    if (m_socket.send_to(boost::asio::buffer(byteString), m_remoteEndpoint) == 0)
                    {
                        Log::error("Failed to send message to client");
                    }
                });
            }

            bool passCommand = Command::read(handledMessage.getData(), *handledMessage.getClient(), *this);

            if (passCommand)
            {
                Log::info("UDPServer: Passed with command: ", data);
            }
            else
            {
                Log::error("UDPServer: Failed with command: ", data);
            }
        }
        catch (const boost::system::system_error& e)
        {
            Log::error("Error receiving message: ", e.what());
        }
    }

    Log::error("Socket is not open!");
}

} // namespace nexilis::boost
