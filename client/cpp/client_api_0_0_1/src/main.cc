#include <nexilis/protocol_manager.hh>
#include <nexilis/af_inet/udp_client.hh>
#include <nexilis/client_api/client_api.hh>

nexilis::ClientAPI::ServerData serverData
{
    //std::string af_inet_server_address;
    "192.168.1.85",

    //uint16_t af_inet_port = 0xFFFF;
    54200,

    //std::string client_username;
    "Valtsuuni"
};


int main()
{
    nexilis::ClientAPI api(serverData);
    nexilis::ProtocolManager protocolManager;

    auto afInet = protocolManager.addProtocol<nexilis::af_inet::UDPClient>(api);

    std::thread serverThread([&afInet](){ afInet.attach(); });

    while (api.IsAfInetUdpReady())
    {
        std::cout << "Not readyy " << std::endl;
    }
    serverThread.join();
}

