#ifndef CHAT_CLIENT_NEXILIS_CLIENT_HH
#define CHAT_CLIENT_NEXILIS_CLIENT_HH

#include <nexilis/client_api.hh>
#include <nexilis/protocol_manager.hh>

#include <nexilis/boost/tcp_client.hh>

class NexilisClient
{
public:
    NexilisClient();
    void start();
private:
    nexilis::ClientAPI::ServerData m_serverData;
    nexilis::ClientAPI m_clientAPI;
    nexilis::ProtocolManager m_protocolManager;
    nexilis::boost::TCPClient m_tcpClient;
};

#endif
