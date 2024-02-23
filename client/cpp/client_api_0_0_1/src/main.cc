#include <nexilis/protocol_manager.hh>
#include <nexilis/af_inet/udp_client.hh>
#include <nexilis/af_inet/tcp_client.hh>
#include <nexilis/client_api.hh>
#include <nexilis/boost/tcp_client.hh>
#include <nexilis/af_unix/sock_stream/client.hh>
#include <nexilis/af_unix/sock_dgram/client.hh>
#include <nexilis/log.hh>
#include <nexilis/packet.hh>

int main()
{
    nexilis::Log::startConsoleLogging();
    nexilis::Log::setLevel(nexilis::logger::LogLevel::DEBUG);

    nexilis::ClientAPI::ServerData serverData("salasana");
    serverData.setInetUDP("192.168.1.85", 54200);
    serverData.setInetTCP("192.168.1.85", 54300);
    serverData.setBoostTCP("192.168.1.85", 12345);
    serverData.setUnixStreamServerPath("/tmp/nexilis");
    serverData.setUnixDgramServerPath("/tmp/nexilis/dgram");

    nexilis::ClientAPI api(serverData);
    nexilis::ProtocolManager protocolManager;

    /// af_inet UDP
    /*
    auto inetUDP = protocolManager.createProtocol<nexilis::af_inet::UDPClient>(api);
    inetUDP.start();
    inetUDP.sendMessage(serverData.getPassword());
    api.waitUntilInetUDPReady();
    auto id = nexilis::Packet::Get::clientId(api);
    inetUDP.sendMessage(id);
    */

    // af_unix STREAM
    /*
    auto unixClient = protocolManager.createProtocol<nexilis::af_unix::sock_stream::Client>(api);
    unixClient.start();
    unixClient.sendMessage(serverData.getPassword());
    api.waitUntilUnixStreamReady();
    std::cout << "UNIX READY" << std::endl;
    auto id = nexilis::Packet::Get::clientId(api);
    unixClient.sendMessage(id);
    */

    // af_unix DGRAM
    //auto unixClient = protocolManager.createProtocol<nexilis::af_unix::Client>();

    /*
    // Boost TCP
    auto boostTCP = protocolManager.createProtocol<nexilis::boost::TCPClient>(api);
    boostTCP.start();
    api.waitUntilBoostTCPReady();
    boostTCP.sendMessage(id);

    // af_inet TCP
    auto inetTCP = protocolManager.createProtocol<nexilis::af_inet::TCPClient>(api);
    inetTCP.start();
    inetTCP.sendMessage(serverData.getPassword());
    api.waitUntilInetTCPReady();
    inetTCP.sendMessage(id);
    */


    while(true){}
    return 0;
}

