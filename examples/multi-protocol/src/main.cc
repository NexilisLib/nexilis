#include <nexilis/af_inet/udp_server.hh>
#include <nexilis/log.hh>
#include <nexilis/protocol_manager.hh>
#include <nexilis/server_manager.hh>
#include <nexilis/af_inet/tcp_server.hh>
#include <nexilis/boost/tcp_server.hh>

int main()
{
    nexilis::Log::startConsoleDebugging();

    nexilis::Authentication auth;
    auth.setMode(nexilis::Authentication::Mode::passwordProtected);
    auth.setCommonPassword("salasana");

    nexilis::ServerManager serverManager;
    serverManager.setAuthentication(auth);

    nexilis::ProtocolManager protocolManager;

    // UDP
    auto udpServer = protocolManager.createProtocol<nexilis::af_inet::UDPServer>();
    udpServer.start();

    // TCP
    //auto tcpServer = protocolManager.createProtocol<nexilis::af_inet::TCPServer>(54300);
    //tcpServer.start();

    // Boost TCP
    auto tcpServer = protocolManager.createProtocol<nexilis::boost::TCPServer>("12345");
    tcpServer.start();

    return 0;
}
