#include <nexilis/nx_data.hh>
#include <nexilis/tcp_client.hh>

namespace nexilis
{

TCPClient::TCPClient(client::ClientAPI& client_api)
    : m_tcpClient(client::nxboost::TCPClient(client_api))
{
}

TCPClient::TCPClient(TCPClient&& other) noexcept
    : m_tcpClient(std::move(other.m_tcpClient))
{
}

TCPClient& TCPClient::operator=(TCPClient&& other) noexcept
{
    if (this != &other)
    {
        m_tcpClient = std::move(other.m_tcpClient);
    }
    return *this;
}

void TCPClient::start()
{
    m_tcpClient.start();
}

void TCPClient::stop()
{
    m_tcpClient.stop();
}

void TCPClient::sendMessage(const nx_data& message)
{
    m_tcpClient.sendMessage(message);
}

void TCPClient::sendMessage(const nx_data& message,
                            const std::function<void()>& callback)
{
    m_tcpClient.sendMessage(message, callback);
}

std::future<void> TCPClient::sendMessageAsync(const nx_data& message)
{
    return m_tcpClient.sendMessageAsync(message);
}

nexilis::client::ProtocolStatus TCPClient::getProtocolStatus()
{
    return m_tcpClient.getProtocolStatus();
}

} // namespace nexilis
