#ifndef CHAT_CLIENT_NEXILIS_CLIENT_HH
#define CHAT_CLIENT_NEXILIS_CLIENT_HH

#include <nexilis/client/client_api.hh>
#include <nexilis/protocol_manager.hh>

#include <nexilis/client/protocol/nxboost/tcp_client.hh>

class NexilisClient
{
public:
    NexilisClient(const std::string& ipAddress, const std::string& userName);
    void start();

    /// Move constructor.
    NexilisClient(NexilisClient&& other);

    /// Move assignment operator.
    NexilisClient& operator=(NexilisClient&& other);

    nexilis::client::ClientAPI& getClientAPI()
    {
        return m_clientAPI;
    }

    nexilis::client::nxboost::TCPClient& getTCPClient()
    {
        return m_tcpClient;
    }

private:
    nexilis::client::ClientAPI::ServerData m_serverData;
    nexilis::client::ClientAPI m_clientAPI;
    nexilis::ProtocolManager m_protocolManager;
    nexilis::client::nxboost::TCPClient m_tcpClient;
};

#endif
