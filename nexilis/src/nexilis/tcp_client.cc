#include <nexilis/nx_data.hh>
#include <nexilis/tcp_client.hh>

namespace nexilis
{

nexilis::client::ClientConfig getServerData(const std::string& ipAddress,
                                            const std::string& password)
{
    nexilis::client::ClientConfig serverData;
    serverData.setPassword(password);
    serverData.setBoostTCPAddress(ipAddress);
    serverData.setBoostUDP(ipAddress);
    serverData.setMode(nexilis::server::AuthenticationMode::password_protected);
    return serverData;
}

TCPClient::TCPClient(ProtocolManager* protocolManager,
                     const std::string& ipAddress,
                     const std::string& userName)
    : m_protocolManager(protocolManager),
      m_clientAPI(getServerData(ipAddress, userName)),
      m_tcpClient(
              m_protocolManager->createProtocol<nexilis::client::nxboost::TCPClient>(
                      m_clientAPI))
{
}

TCPClient::TCPClient(TCPClient&& other) noexcept
    : m_protocolManager(std::move(other.m_protocolManager)),
      m_clientAPI(std::move(other.m_clientAPI)),
      m_tcpClient(std::move(other.m_tcpClient))
{
}

TCPClient& TCPClient::operator=(TCPClient&& other) noexcept
{
    if (this != &other)
    {
        m_clientAPI = std::move(other.m_clientAPI);
        m_protocolManager = std::move(other.m_protocolManager);
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
