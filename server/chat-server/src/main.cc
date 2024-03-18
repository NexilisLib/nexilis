// nexilis libs
#include "nexilis/logger/log_level.hh"
#include <nexilis/log.hh>
#include <nexilis/protocol_manager.hh>
#include <nexilis/server_manager.hh>
#include <nexilis/mysql/database.hh>

// nexilis protocols
#include <nexilis/boost/tcp_server.hh>
#include <nexilis/boost/udp_server.hh>

int main()
{
    nexilis::Log::startConsoleDebugging();

    nexilis::Authentication auth;
    auth.setMode(nexilis::Authentication::Mode::passwordProtected);
    auth.setCommonPassword("salasana");

    nexilis::ServerManager serverManager;
    serverManager.setAuthentication(auth);

    nexilis::ProtocolManager protocolManager;

    // Boost TCP
    auto boostTCPServer = protocolManager.createProtocol<nexilis::boost::TCPServer>(12348);
    boostTCPServer.start();
    std::cout << "nexilis server setup ready" << std::endl;

    return 0;
}
