#include "nexilis/loggable.hh"
#include <boost/asio/ip/address.hpp>
#include <memory>
#include <mutex>
#include <nexilis/boost/udp_client.hh>
#include <nexilis/log.hh>

namespace nexilis::boost
{

UDPClient::UDPClient(ClientAPI& clientApi)
    : ClientProtocol(&clientApi),
      Loggable("boost::UDPClient", __FILE__),
      m_ioContext(std::make_unique<boost::asio::io_context>()),
      m_mutex(std::make_unique<std::mutex>()),
      m_endPoint(boost::asio::ip::make_address(clientApi.getBoostUDPServerAddress()), clientApi.getBoostUDPServerPortNumber()),
      m_remoteEndpoint(boost::asio::ip::udp::endpoint()),
      m_socket(*m_ioContext)
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
}

UDPClient::UDPClient(UDPClient&& other)
    : Protocol(std::move(other)),
      ClientProtocol(std::move(other)),
      Loggable(std::move(other)),
      m_ioContextThread(std::move(other.m_ioContextThread)),
      m_ioContext(std::move(other.m_ioContext)),
      m_mutex(std::move(other.m_mutex)),
      m_endPoint(std::move(other.m_endPoint)),
      m_remoteEndpoint(std::move(other.m_remoteEndpoint)),
      m_socket(std::move(other.m_socket)),
      m_receiveBuffer(std::move(other.m_receiveBuffer))
{
    other.m_ioContext = nullptr;
}

UDPClient& UDPClient::operator=(UDPClient&& other)
{
    if (this != &other)
    {
        Protocol::operator=(std::move(other));
        ClientProtocol::operator=(std::move(other));
        Loggable::operator=(std::move(other));
        m_ioContextThread = std::move(other.m_ioContextThread);
        m_ioContext = std::move(other.m_ioContext);
        m_mutex = std::move(other.m_mutex);
        m_endPoint = std::move(other.m_endPoint);
        m_remoteEndpoint = std::move(other.m_remoteEndpoint);
        m_socket = std::move(other.m_socket);
        m_receiveBuffer = std::move(other.m_receiveBuffer);

        other.m_ioContext = nullptr;
    }
    return *this;
}

void UDPClient::start()
{
    m_socket.open(boost::asio::ip::udp::v4());

    m_ioContextThread = std::thread([this](){ m_ioContext->run(); });

    Log::info("io context ready");

    info("io_context ready");
    infoExtra("io context is really ready");

    if (m_socket.is_open())
    {
        std::lock_guard<std::mutex> lock(*m_mutex);

        m_socket.async_receive_from(boost::asio::buffer(m_receiveBuffer), m_remoteEndpoint,
                                    [this](const boost::system::error_code& error, std::size_t bytesTransferred)
                                    {
                                        if (!error)
                                        {
                                            std::string receivedMessage(m_receiveBuffer.data(), bytesTransferred);
                                            Log::info("Received from server: ", receivedMessage);
                                        }
                                        else
                                        {
                                            Log::error("boost::UDPClient::start(): Error receiving message, reason: ", error.message());
                                        }
                                    });
    }
    else
    {
        Log::error("boost::UDPClient::start(): socket is not open");
    }
}

void UDPClient::sendMessage(const std::string& message)
{
    send(message);
}

void UDPClient::sendMessage(const std::vector<uint8_t>& message)
{
    std::string msg = reinterpret_cast<const char*>(message.data());
    send(msg);
}

void UDPClient::send(const std::string& message)
{
    if (m_socket.is_open())
    {
        m_socket.send_to(boost::asio::buffer(message), m_endPoint);
    }
    else
    {
        Log::error("UDPClient::send(): boost::UDPClient socket is not open");
    }
}

} // namespace nexilis::boost
