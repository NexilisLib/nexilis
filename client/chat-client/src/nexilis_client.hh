#ifndef CHAT_CLIENT_NEXILIS_CLIENT_HH
#define CHAT_CLIENT_NEXILIS_CLIENT_HH

#include <nexilis/client_api.hh>
#include <nexilis/protocol_manager.hh>

#include <nexilis/boost/tcp_client.hh>
#include <nexilis/boost/udp_client.hh>

class NexilisClient
{
public:
    NexilisClient(const std::string& ipAddress, const std::string& userName);
    void start();

    /// Move constructor.
    NexilisClient(NexilisClient&& other);

    /// Move assignment operator.
    NexilisClient& operator=(NexilisClient&& other);

    nexilis::ClientAPI& getClientAPI()
    {
        return m_clientAPI;
    }

    nexilis::BoostTCPClient& getTCPClient()
    {
        return m_tcpClient;
    }

    nexilis::BoostUDPClient& getUDPClient()
    {
        return m_udpClient;
    }

private:
    nexilis::ClientAPI::ServerData m_serverData;
    nexilis::ClientAPI m_clientAPI;
    nexilis::ProtocolManager m_protocolManager;
    nexilis::BoostUDPClient m_udpClient;
    nexilis::BoostTCPClient m_tcpClient;
};

#endif
