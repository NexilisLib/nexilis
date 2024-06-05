#include "nexilis_client.hh"
#include "debug.hh"

#include <nexilis/common/util.hh>
#include <nexilis/packet.hh>

nexilis::ClientAPI::ServerData getServerData(const std::string& ipAddress)
{
    nexilis::ClientAPI::ServerData serverData("salasana");
    serverData.setBoostTCP(ipAddress, 12348);
    serverData.setBoostUDP(ipAddress, 12347);
    return serverData;
}

NexilisClient::NexilisClient(const std::string& ipAddress)
    : m_serverData(getServerData(ipAddress)),
      m_clientAPI(m_serverData),
      m_udpClient(m_protocolManager.createProtocol<nexilis::BoostUDPClient>(m_clientAPI)),
      m_tcpClient(m_protocolManager.createProtocol<nexilis::BoostTCPClient>(m_clientAPI))
{
}

NexilisClient::NexilisClient(NexilisClient&& other)
    : m_serverData(std::move(other.m_serverData)),
      m_clientAPI(std::move(other.m_clientAPI)),
      m_protocolManager(std::move(other.m_protocolManager)),
      m_udpClient(std::move(other.m_udpClient)),
      m_tcpClient(std::move(other.m_tcpClient))
{
}

NexilisClient& NexilisClient::operator=(NexilisClient&& other)
{
    if (this != &other)
    {
        m_serverData = std::move(other.m_serverData);
        m_clientAPI = std::move(other.m_clientAPI);
        m_protocolManager = std::move(other.m_protocolManager);
        m_udpClient = std::move(other.m_udpClient);
        m_tcpClient = std::move(other.m_tcpClient);
    }
    return *this;
}

void NexilisClient::start()
{
    m_udpClient.start();
    m_tcpClient.start();

    // Convert the passphrase into nexilis format (std::vector<uint8_t>).
    auto message = nexilis::Util::convertToByteVector(m_serverData.getPassword().c_str(), m_serverData.getPassword().size());
    m_tcpClient.sendMessage(message);
    m_clientAPI.waitUntilBoostTCPReady();

    debug(nexilis::Util::getDateAndTime());
}
