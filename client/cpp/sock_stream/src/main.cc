#include <nexilis/client_api/client_api.hh>
#include <nexilis/protocol_manager.hh>
#include <nexilis/af_unix/sock_stream/client.hh>

int main()
{
    nexilis::ClientAPI::ServerData serverData("salasana");
    serverData.setUnixSocketServerPath("/tmp/nexilis");

    nexilis::ClientAPI api(serverData);

    nexilis::ProtocolManager protocolManager;

    auto client = protocolManager.createProtocol<nexilis::af_unix::sock_stream::Client>(api);

    client.start();
}
