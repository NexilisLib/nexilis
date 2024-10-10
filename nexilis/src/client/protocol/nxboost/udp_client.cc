#include <nexilis/client/protocol/nxboost/udp_client.hh>
#include <nexilis/logger/log.hh>
#include <nexilis/nexilis_macros.hh>

#include <boost/asio/ip/address.hpp>

#include <mutex>

namespace nexilis::client::nxboost
{

UDPClient::UDPClient(ClientAPI& clientApi)
    : ClientProtocol(&clientApi),
      Loggable("boost::UDPClient", __FILE__),
      m_ioContext(std::make_unique<boost::asio::io_context>()),
      m_mutex(std::make_unique<std::mutex>()),
      m_remoteEndpoint(boost::asio::ip::make_address(clientApi.getBoostUDPServerAddress()), clientApi.getBoostUDPServerPortNumber()),
      m_socket(*m_ioContext),
      m_receiveBuffer(NEXILIS_BUFFER)
{
}

UDPClient::~UDPClient()
{
    m_socket.close();
    m_ioContext->stop();

    if (m_ioContextThread.joinable())
    {
        m_ioContextThread.join();
    }

    if (m_receiveMessageThread.joinable())
    {
        m_receiveMessageThread.join();
    }
}

UDPClient::UDPClient(UDPClient&& other)
    : Protocol(std::move(other)),
      ClientProtocol(std::move(other)),
      Loggable(std::move(other)),
      m_ioContextThread(std::move(other.m_ioContextThread)),
      m_receiveMessageThread(std::move(other.m_receiveMessageThread)),
      m_ioContext(std::move(other.m_ioContext)),
      m_mutex(std::move(other.m_mutex)),
      m_remoteEndpoint(std::move(other.m_remoteEndpoint)),
      m_socket(std::move(other.m_socket)),
      m_receiveBuffer(std::move(other.m_receiveBuffer))
{
    other.m_ioContext = nullptr;
    other.m_mutex = nullptr;
}

UDPClient& UDPClient::operator=(UDPClient&& other)
{
    if (this != &other)
    {
        Protocol::operator=(std::move(other));
        ClientProtocol::operator=(std::move(other));
        Loggable::operator=(std::move(other));
        m_ioContextThread = std::move(other.m_ioContextThread);
        m_receiveMessageThread = std::move(other.m_receiveMessageThread);
        m_ioContext = std::move(other.m_ioContext);
        m_mutex = std::move(other.m_mutex);
        m_remoteEndpoint = std::move(other.m_remoteEndpoint);
        m_socket = std::move(other.m_socket);
        m_receiveBuffer = std::move(other.m_receiveBuffer);

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
}

void UDPClient::receiveLoop()
{
    while (m_socket.is_open())
    {
        std::size_t receivedBytes = m_socket.receive_from(boost::asio::buffer(m_receiveBuffer), m_remoteEndpoint);
        Log::info("Received bytes: ", receivedBytes);

        auto byteVector = Util::convertToByteVector(m_receiveBuffer.data(), receivedBytes);
        ClientProtocol::getClientAPI()->readMessage(byteVector);
    }
}

void UDPClient::sendMessage(const nx_data& payload)
{
    if (m_socket.is_open())
    {
        m_socket.send_to(boost::asio::buffer(payload), m_remoteEndpoint);
        Log::debug("sent message to server");
    }
    else
    {
        Log::error("UDPClient::send(): boost::UDPClient socket is not open");
    }
}

} // namespace nexilis
