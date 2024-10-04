#include <boost/asio/ip/address.hpp>
#include <boost/system/system_error.hpp>
#include <nexilis/boost/udp_server.hh>
#include <nexilis/command.hh>
#include <nexilis/log.hh>

namespace nexilis
{

BoostUDPServer::BoostUDPServer(int port)
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

BoostUDPServer::~BoostUDPServer()
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

BoostUDPServer::BoostUDPServer(BoostUDPServer&& other)
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

BoostUDPServer& BoostUDPServer::operator=(BoostUDPServer&& other)
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

void BoostUDPServer::start()
{
    m_ioContextThread = std::thread([this]()
                                    { m_ioContext->run(); });

    m_receiveThread = std::thread(&BoostUDPServer::receiveFromClients, this);
}

void BoostUDPServer::receiveFromClients()
{
    std::lock_guard<std::mutex> lock(*m_mutex);

    while (m_socket.is_open())
    {
        try
        {
            m_receiveBuffer.clear();

            m_socket.receive_from(boost::asio::buffer(m_receiveBuffer), m_remoteEndpoint);

            std::string address = m_remoteEndpoint.address().to_string();
            uint16_t port = m_remoteEndpoint.port();
            Log::info("Received from ", address, " port ", port);

            auto handledMessage = getMessageHandler().readMessage(address, m_receiveBuffer, port, Command::getAuthentication());

            if (!handledMessage.getClient()->isBoostUDPSet())
            {
                handledMessage.getClient()->setBoostUDPSend([this](const std::vector<uint8_t>& bytes)
                                                            {
                    if (m_socket.send_to(boost::asio::buffer(bytes), m_remoteEndpoint) == 0)
                    {
                        Log::error("Failed to send message to client");
                    } });
            }

            Command::Result passCommand = Command::read(handledMessage.getData(), *handledMessage.getClient(), *this, handledMessage.getMessageId());

            switch (passCommand)
            {
                case Command::Result::success:
                    Log::info("UDPServer: Passed");
                    break;

                default:
                    Log::error("UDPServer: Something FAILED");
            }
        }
        catch (const boost::system::system_error& e)
        {
            Log::error("Error receiving message: ", e.what());
        }
    }

    Log::error("Socket is not open!");
}

} // namespace nexilis
