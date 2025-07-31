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
    settings.setMode(nexilis::server::AuthenticationMode::password_protected);
    settings.setPassphrase("salasana");
    settings.setRootPassword("root");

    auto server = protocol_manager.createProtocol<nexilis::server::nxboost::TCPServer>(settings);
    server.start();

    std::this_thread::sleep_for(std::chrono::seconds(1));

    nexilis::client::ServerData server_data;
    server_data.setPassword("salasana");
    server_data.setBoostTCPAddress("127.0.0.1");

    nexilis::client::ClientAPI client_api(server_data);
    auto client = protocol_manager.createProtocol<nexilis::client::nxboost::TCPClient>(client_api);
    client.start();

    std::this_thread::sleep_for(std::chrono::seconds(1));
}
