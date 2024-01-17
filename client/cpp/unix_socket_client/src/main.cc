#include <nexilis/af_unix/unix_socket_client.hh>
#include <nexilis/client_api/client_api.hh>
#include <nexilis/protocol_manager.hh>

int main()
{
    nexilis::ClientAPI::ServerData serverData("salasana");
    serverData.setUnixSocketServerPath("/tmp/nexilis");

    nexilis::ClientAPI api(serverData);

    nexilis::ProtocolManager protocolManager;

    auto unix_client = protocolManager.createProtocol<nexilis::af_unix::UnixSocketClient>(api);

    //unix_client.start();

    while (api.isUnixSocketClientReady())
    {
        std::cout << "Not ready for action!" << std::endl;
    }

    return 0;
}
