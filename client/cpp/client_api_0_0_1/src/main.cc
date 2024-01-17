#include <nexilis/protocol_manager.hh>
#include <nexilis/af_inet/udp_client.hh>
#include <nexilis/client_api/client_api.hh>

int main()
{
    //nexilis::ClientAPI::ServerData serverData("192.168.1.85", static_cast<uint16_t>(54200), "Valtsuuni");
    nexilis::ClientAPI::ServerData serverData;
    nexilis::ClientAPI api(serverData);
    nexilis::ProtocolManager protocolManager;

    auto afInet = protocolManager.createProtocol<nexilis::af_inet::UDPClient>(api);

    //std::thread serverThread([&afInet](){ afInet.attach(); });
    afInet.attach();

    while (api.IsInetUdpReady())
    {
        //std::cout << "Not readyy " << std::endl;
    }
    //serverThread.join();
}

