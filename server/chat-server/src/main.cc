// nexilis libs
#include <nexilis/logger/log.hh>
#include <nexilis/mysql/database.hh>
#include <nexilis/protocol_manager.hh>
#include <nexilis/server/room.hh>
#include <nexilis/server/room_storage.hh>

// nexilis protocols
#include <nexilis/server/protocol/nxboost/tcp_server.hh>

#include <iostream>

int main()
{
    nexilis::Log::startConsoleDebugging();

    using namespace nexilis::server;

    Settings settings;
    settings.setMode(Settings::AuthenticationMode::passwordProtected);
    settings.setPassphrase("salasana");
    settings.setRootPassword("root");

    nexilis::ProtocolManager protocolManager;

    // Boost TCP
    auto boostTCPServer = protocolManager.createProtocol<nxboost::TCPServer>(settings, 12348);
    boostTCPServer.start();
    std::cout << "nexilis boost TCP ready" << std::endl;

    std::cout << "SERVER READY, looping main thread" << std::endl;
    while (true)
    {
    }
}
