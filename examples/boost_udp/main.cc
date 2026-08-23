#include <nexilis/logger/log.hh>
#include <nexilis/protocol_manager.hh>

#include <nexilis/client/protocol/nxboost/udp_client.hh>
#include <nexilis/server/protocol/nxboost/udp_server.hh>

#include <nexilis/client/client_api.hh>
#include <nexilis/client/packet.hh>

#include <chrono>
#include <iostream>
#include <thread>

int main()
{
    nexilis::Log::startConsoleDebugging();

    nexilis::ProtocolManager protocol_manager;

    // Server.
    nexilis::server::ServerConfig settings;
    settings.setMode(nexilis::server::AuthenticationMode::password_protected);
    settings.setPassphrase("salasana");
    settings.setRootPassword("root");

    auto server = protocol_manager.createProtocol<nexilis::server::nxboost::UDPServer>(settings);
    server.start();

    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Client.
    nexilis::client::ClientConfig server_data;
    server_data.setPassword("salasana");
    server_data.setBoostUDPAddress("127.0.0.1");
    server_data.setMode(nexilis::server::AuthenticationMode::password_protected);

    nexilis::client::ClientAPI client_api(server_data);
    auto client = protocol_manager.createProtocol<nexilis::client::nxboost::UDPClient>(client_api);
    client.start();

    std::this_thread::sleep_for(std::chrono::seconds(1));

    client.stop();
    std::cout << "Nexilis boost UDP client stopped" << std::endl;

    server.stop();
}
