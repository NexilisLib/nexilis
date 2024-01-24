#include <nexilis/protocol_manager.hh>
#include <nexilis/af_inet/udp_client.hh>
#include <nexilis/client_api/client_api.hh>

int main()
{
    nexilis::ClientAPI::ServerData serverData("salasana");
    serverData.setInetUDP("192.168.1.85", 54200);

    nexilis::ClientAPI api(serverData);
    nexilis::ProtocolManager protocolManager;

    auto afInet = protocolManager.createProtocol<nexilis::af_inet::UDPClient>(api);

    afInet.start();

    while (true) {}

    return 0;
}

