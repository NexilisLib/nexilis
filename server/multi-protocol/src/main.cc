#include <nexilis/af_inet/udp_server.hh>
#include <nexilis/log.hh>
#include <nexilis/protocol_manager.hh>
#include <nexilis/server_manager.hh>
#include <nexilis/af_inet/tcp_server.hh>
#include <nexilis/boost/tcp_server.hh>
#include <nexilis/af_unix/sock_stream/server.hh>
#include <nexilis/af_unix/sock_dgram/server.hh>

int main()
{
    nexilis::Log::startConsoleDebugging();

    nexilis::Authentication auth;
    auth.setMode(nexilis::Authentication::Mode::passwordProtected);
    auth.setCommonPassword("salasana");

    nexilis::ServerManager serverManager;
    serverManager.setAuthentication(auth);

    nexilis::ProtocolManager protocolManager;

    // af_unix STREAM
    /*
    auto unixServer = protocolManager.createProtocol<nexilis::af_unix::sock_stream::Server>("/tmp/nexilis");
    unixServer.start();
    */

    // af_unix DGRAM
    auto unixServer = protocolManager.createProtocol<nexilis::af_unix::sock_dgram::Server>("/tmp/nexilis_dgram");
    unixServer.start();

    /*
    // af_inet TCP
    auto inetTCPServer = protocolManager.createProtocol<nexilis::af_inet::TCPServer>(54300);
    inetTCPServer.start();

    // af_inet UDP
    auto inetUDPServer = protocolManager.createProtocol<nexilis::af_inet::UDPServer>();
    inetUDPServer.start();

    // Boost TCP
    auto boostTCPServer = protocolManager.createProtocol<nexilis::boost::TCPServer>(12345);
    boostTCPServer.start();
    */

    return 0;
}
