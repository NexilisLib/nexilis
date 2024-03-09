// nexilis libs
#include <nexilis/log.hh>
#include <nexilis/protocol_manager.hh>
#include <nexilis/server_manager.hh>
#include <nexilis/mysql/database.hh>

// nexilis protocols
#include <nexilis/af_inet/udp_server.hh>
#include <nexilis/af_inet/tcp_server.hh>
#include <nexilis/af_unix/sock_stream/server.hh>
#include <nexilis/af_unix/sock_dgram/server.hh>
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

    nexilis::mysql::Database::ConnectionData connectionData("0.0.0.0", "root", "my_password", "my_database");
    nexilis::mysql::Database database(connectionData);

    database.executeQuery("CREATE TABLE cpp (id INT AUTO_INCREMENT PRIMARY KEY, name VARCHAR(255))");

    // af_unix STREAM
    /*
    auto unixServer = protocolManager.createProtocol<nexilis::af_unix::sock_stream::Server>("/tmp/nexilis");
    unixServer.start();
    */

    // af_unix DGRAM
    /*
    auto unixServer = protocolManager.createProtocol<nexilis::af_unix::sock_dgram::Server>("/tmp/nexilis_dgram");
    unixServer.start();
    */

    /*
    // af_inet TCP
    auto inetTCPServer = protocolManager.createProtocol<nexilis::af_inet::TCPServer>(54300);
    inetTCPServer.start();
    */

    /*
    // af_inet UDP
    auto inetUDPServer = protocolManager.createProtocol<nexilis::af_inet::UDPServer>();
    inetUDPServer.start();
    */

    // Boost TCP
    auto boostTCPServer = protocolManager.createProtocol<nexilis::boost::TCPServer>(12348);
    boostTCPServer.start();
    std::cout << "Boost TCP Done" << std::endl;

    // Boost UDP
    /*
    auto boostUDPServer = protocolManager.createProtocol<nexilis::boost::UDPServer>(12346);
    boostUDPServer.start();
    std::cout << "Boost UDP Done" << std::endl;
    */

    std::cout << std::endl;
    std::cout << "nexilis server setup ready" << std::endl;

    while (true) {}


    return 0;
}
