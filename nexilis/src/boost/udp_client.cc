#include <boost/asio/ip/address.hpp>
#include <nexilis/boost/udp_client.hh>
#include <nexilis/log.hh>

namespace nexilis::boost
{

UDPClient::UDPClient(ClientAPI& clientApi) :
    ClientProtocol(&clientApi),
    m_ioContext(std::make_unique<::boost::asio::io_context>()),
    m_endPoint(::boost::asio::ip::make_address(clientApi.getBoostUDPServerAddress()), clientApi.getBoostUDPServerPortNumber()),
    m_remoteEndpoint(::boost::asio::ip::udp::endpoint()),
    m_socket(*m_ioContext)
{
}

UDPClient::UDPClient(UDPClient&& other) :
    Protocol(std::move(other)),
    ClientProtocol(std::move(other)),
    m_ioContext(std::move(other.m_ioContext)),
    m_endPoint(std::move(other.m_endPoint)),
    m_remoteEndpoint(std::move(other.m_remoteEndpoint)),
    m_socket(std::move(other.m_socket)),
    m_receiveBuffer(std::move(other.m_receiveBuffer))
{
}

UDPClient& UDPClient::operator=(UDPClient&& other)
{
    if (this != &other)
    {
        Protocol::operator=(std::move(other));
        ClientProtocol::operator=(std::move(other));
        m_ioContext = std::move(other.m_ioContext);
        m_endPoint = std::move(other.m_endPoint);
        m_remoteEndpoint = std::move(other.m_remoteEndpoint);
        m_socket = std::move(other.m_socket);
        m_receiveBuffer = std::move(other.m_receiveBuffer);
    }
    return *this;
}

void UDPClient::start()
{
    m_socket.async_receive_from(::boost::asio::buffer(m_receiveBuffer), m_remoteEndpoint,
    [this](const ::boost::system::error_code& error, std::size_t bytesTransferred)
    {
        if (!error)
        {
            std::string receivedMessage(m_receiveBuffer.data(), bytesTransferred);
            Log::info(logName(), "Received from server: ", receivedMessage);
        }
        else
        {
            Log::error(logName(), "Error receiving message, reason: ", error.message());
        }
    });
}

void UDPClient::sendMessage(const std::string& message)
{
    m_socket.send_to(::boost::asio::buffer(message), m_endPoint);
}

void UDPClient::sendMessage(const std::vector<uint8_t>& message)
{
    std::string msg = reinterpret_cast<const char*>(message.data());
    m_socket.send_to(::boost::asio::buffer(msg), m_endPoint);
}

}
