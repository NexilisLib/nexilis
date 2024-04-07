#include "nexilis_client.hh"
#include "debug.hh"

#include <nexilis/common/util.hh>
#include <nexilis/packet.hh>

nexilis::ClientAPI::ServerData getServerData()
{
    nexilis::ClientAPI::ServerData serverData("salasana");
    serverData.setBoostTCP("192.168.1.85", 12348);
    serverData.setBoostUDP("192.168.1.85", 12346);
    return serverData;
}

NexilisClient::NexilisClient()
    : m_serverData(getServerData()),
      m_clientAPI(m_serverData),
      m_udpClient(m_protocolManager.createProtocol<nexilis::boost::UDPClient>(m_clientAPI)),
      m_tcpClient(m_protocolManager.createProtocol<nexilis::boost::TCPClient>(m_clientAPI))
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

    std::cout << "UDPClient started!" << std::endl;

    m_udpClient.sendMessage(m_serverData.getPassword());

    std::cout << "send message to server "<< std::endl;

    /*
    m_tcpClient.start();
    m_tcpClient.sendMessage(m_serverData.getPassword());

    m_clientAPI.waitUntilBoostTCPReady();

    std::cout << "TCP READY" << std::endl;
    */

    m_clientAPI.waitUntilBoostUDPReady();

    debug("Boost TCP connection ready");
    debug(nexilis::Util::getDateAndTime());
}
