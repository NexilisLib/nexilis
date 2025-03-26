#include <nexilis/client/protocol/nxboost/udp_client.hh>
#include <nexilis/logger/log.hh>
#include <nexilis/nexilis_constants.hh>

#include <boost/asio/ip/address.hpp>

#include <mutex>

namespace nexilis::client::nxboost
{

UDPClient::UDPClient(ClientAPI& clientApi)
    : ClientProtocol(&clientApi),
      m_ioContext(std::make_unique<boost::asio::io_context>()),
      m_mutex(std::make_unique<std::mutex>()),
      m_socket(*m_ioContext),
      m_receiveBuffer(NEXILIS_BUFFER)
{
    auto port = Util::readPortFromFile(Protocol::Type::BOOST_UDP_SERVER);

    if (!port)
    {
        Log::error("Boost UDP port unspecified");
        return;
    }

    m_remoteEndpoint = boost::asio::ip::udp::endpoint(
            boost::asio::ip::make_address(clientApi.getBoostTCPServerAddress()),
            *port);
}

UDPClient::~UDPClient()
{
    stop();
}

UDPClient::UDPClient(UDPClient&& other)
    : Protocol(std::move(other)),
      ClientProtocol(std::move(other)),
      m_ioContextThread(std::move(other.m_ioContextThread)),
      m_receiveMessageThread(std::move(other.m_receiveMessageThread)),
      m_ioContext(std::move(other.m_ioContext)),
      m_mutex(std::move(other.m_mutex)),
      m_socket(std::move(other.m_socket)),
      m_receiveBuffer(std::move(other.m_receiveBuffer)),
      m_remoteEndpoint(std::move(other.m_remoteEndpoint))
{
    other.m_ioContext = nullptr;
    other.m_mutex = nullptr;
}

UDPClient& UDPClient::operator=(UDPClient&& other)
{
    if (this != &other)
    {
        m_ioContextThread = std::move(other.m_ioContextThread);
        m_receiveMessageThread = std::move(other.m_receiveMessageThread);
        m_ioContext = std::move(other.m_ioContext);
        m_mutex = std::move(other.m_mutex);
        m_socket = std::move(other.m_socket);
        m_receiveBuffer = std::move(other.m_receiveBuffer);
        m_remoteEndpoint = std::move(other.m_remoteEndpoint);

        Protocol::operator=(std::move(other));
        ClientProtocol::operator=(std::move(other));

        other.m_ioContext = nullptr;
        other.m_mutex = nullptr;
    }
    return *this;
}

void UDPClient::start()
{
    m_ioContextThread = std::thread([this]()
                                    { m_ioContext->run(); });
    m_socket.open(boost::asio::ip::udp::v4());
    m_receiveMessageThread = std::thread(&UDPClient::receiveLoop, this);

    ClientProtocol::start(getType());
}

void UDPClient::stop()
{
    if (m_socket.is_open())
    {
        m_socket.close();
    }

    if (m_ioContext)
    {
        Log::debug("BoostUDPClient io context stopped");
        m_ioContext->stop();
    }

    if (m_ioContextThread.joinable())
    {
        Log::debug("BoostUDPClient io context thread stopped");
        m_ioContextThread.join();
    }

    if (m_receiveMessageThread.joinable())
    {
        Log::debug("BoostUDPClient receive message thread stopped");
        m_receiveMessageThread.join();
    }
}

void UDPClient::receiveLoop()
{
    while (m_socket.is_open())
    {
        std::size_t receivedBytes = m_socket.receive_from(boost::asio::buffer(m_receiveBuffer), m_remoteEndpoint);
        Log::info("Received bytes: ", receivedBytes);

        auto byteVector = Util::convertToByteVector(m_receiveBuffer.data(), receivedBytes);
        auto result = ClientProtocol::getClientAPI()->readMessage(byteVector);
        if (result == ClientAPI::ReadResult::success)
        {
            Log::debug("BoostUDPClient: Message read successfully");
        }
        else
        {
            Log::error("Received unexpected message: ");
            Util::debugUint8Vector(byteVector);
        }
    }
}

void UDPClient::sendMessage(const nx_data& payload)
{
    if (m_socket.is_open())
    {
        size_t bytes_sent = m_socket.send_to(boost::asio::buffer(payload), m_remoteEndpoint);
        Log::debug("Sent ", bytes_sent, " bytes to server");
    }
    else
    {
        Log::error("UDPClient::send(): boost::UDPClient socket is not open");
    }
}

} // namespace nexilis::client::nxboost
