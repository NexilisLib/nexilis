#include "nexilis_client.hh"
#include "debug.hh"

#include <nexilis/util.hh>
#include <nexilis/client/client_api.hh>
#include <nexilis/client/packet.hh>

#include <iostream>

nexilis::client::ClientAPI::ServerData getServerData(const std::string& ipAddress, const std::string& userName)
{
    nexilis::client::ClientAPI::ServerData serverData;
    serverData.setPassword("salasana");
    serverData.setUserName(userName);
    serverData.setBoostTCP(ipAddress, 12348);
    return serverData;
}

NexilisClient::NexilisClient(const std::string& ipAddress, const std::string& userName)
    : m_serverData(getServerData(ipAddress, userName)),
      m_clientAPI(m_serverData),
      m_tcpClient(m_protocolManager.createProtocol<nexilis::client::nxboost::TCPClient>(m_clientAPI))
{
}

NexilisClient::NexilisClient(NexilisClient&& other)
    : m_serverData(std::move(other.m_serverData)),
      m_clientAPI(std::move(other.m_clientAPI)),
      m_protocolManager(std::move(other.m_protocolManager)),
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
        m_tcpClient = std::move(other.m_tcpClient);
    }
    return *this;
}

void NexilisClient::start()
{
    std::cout << "Nexilisclient start called" << std::endl;
    m_tcpClient.start();
    std::cout << "TCP client started" << std::endl;

    // Convert the passphrase into nexilis format (std::vector<uint8_t>).
    auto message = nexilis::Util::convertToByteVector(m_serverData.getPassword().c_str(), m_serverData.getPassword().size());
    m_tcpClient.sendMessage(message);
    std::cout << "Sent password" << std::endl;
    m_clientAPI.waitUntilBoostTCPReady();

    // Set username.
    std::string username = "niih";
    m_tcpClient.sendMessage(nexilis::client::Packet::Set::username(username));

    debug(nexilis::Util::getDateAndTime());
}
