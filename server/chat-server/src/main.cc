#include <nexilis/logger/log.hh>
#include <nexilis/mysql/database.hh>
#include <nexilis/protocol_manager.hh>
#include <nexilis/server/protocol/nxboost/tcp_server.hh>
#include <nexilis/server/room.hh>
#include <nexilis/server/room_storage.hh>
#include <nexilis/server/runtime.hh>

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

    auto condition = [](size_t)
    { return true; };
    auto f = std::function<bool()>([]()
                                   {
        std::cout << "Updating chat server" << std::endl;
        return true; });

    auto server_runtime = std::thread([&condition, &f]()
                                      { nexilis::server::runtime(condition, f, 1); });

    server_runtime.detach();
}
