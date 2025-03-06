#include <nexilis/logger/log.hh>
#include <nexilis/protocol_manager.hh>

#include <nexilis/client/protocol/nxboost/tcp_client.hh>
#include <nexilis/server/protocol/nxboost/tcp_server.hh>

#include <nexilis/client/client_api.hh>
#include <nexilis/client/packet.hh>

int main()
{
    nexilis::Log::startConsoleDebugging();

    nexilis::ProtocolManager protocol_manager;

    nexilis::server::Settings settings;
    settings.setMode(nexilis::server::Settings::AuthenticationMode::passwordProtected);
    settings.setPassphrase("salasana");
    settings.setRootPassword("root");

    auto server = protocol_manager.createProtocol<nexilis::server::nxboost::TCPServer>(settings, 11223);
    server.start();

    nexilis::client::ClientAPI::ServerData server_data;
    server_data.setPassword("salasana");
    server_data.setUserName("example_user");
    server_data.setBoostTCP("127.0.0.1", 11223);

    nexilis::client::ClientAPI client_api(server_data);
    auto client = protocol_manager.createProtocol<nexilis::client::nxboost::TCPClient>(client_api);
    client.start();

    while (true)
    {
    }
}
