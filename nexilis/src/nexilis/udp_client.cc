#include <nexilis/udp_client.hh>

namespace nexilis
{

UDPClient::UDPClient(client::ClientAPI& api)
    : m_udpClient(api)
{
}

UDPClient::UDPClient(UDPClient&& other) noexcept
    : m_udpClient(std::move(other.m_udpClient))
{
}

UDPClient& UDPClient::operator=(UDPClient&& other) noexcept
{
    if (this != &other)
    {
        m_udpClient = std::move(other.m_udpClient);
    }
    return *this;
}

void UDPClient::start()
{
    m_udpClient.start();
}

void UDPClient::stop()
{
    m_udpClient.stop();
}

void UDPClient::sendMessage(const nx_data& message)
{
    m_udpClient.sendMessage(message);
}

void UDPClient::sendMessage(const nx_data& message, const std::function<void()>& callback)
{
    m_udpClient.sendMessage(message, callback);
}

std::future<void> UDPClient::sendMessageAsync(const nx_data& message)
{
    return m_udpClient.sendMessageAsync(message);
}

} // namespace nexilis
