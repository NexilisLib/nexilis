#include <nexilis/af_inet/udp_server.hh>
#include <nexilis/log.hh>
#include <nexilis/protocol_manager.hh>
#include <nexilis/websocket/websocket.hh>
#include <nexilis/server_manager.hh>

int main()
{
    nexilis::Log::startConsoleLogging();
    nexilis::Log::setLevel(nexilis::LogLevel::DEBUG);

    nexilis::Authentication auth;
    auth.setMode(nexilis::Authentication::Mode::passwordProtected);
    auth.setCommonPassword("salasana");

    nexilis::ServerManager serverManager;
    serverManager.setAuthentication(auth);

    nexilis::ProtocolManager protocolManager;

    auto udpServer = protocolManager.createProtocol<nexilis::af_inet::UDPServer>();

    udpServer.start();

    return 0;
}
