// nexilis libs
#include <nexilis/protocol_manager.hh>
#include <nexilis/client_api.hh>
#include <nexilis/log.hh>
#include <nexilis/packet.hh>

// nexilis protocols
#include <nexilis/boost/tcp_client.hh>
#include <nexilis/boost/udp_client.hh>
#include <nexilis/af_unix/sock_stream/client.hh>
#include <nexilis/af_unix/sock_dgram/client.hh>
#include <nexilis/af_inet/udp_client.hh>
#include <nexilis/af_inet/tcp_client.hh>

#define HOME_ADDRESS "192.168.1.85"
#define LAPTOP "192.168.13.74"
#define AT_HOME true

int main()
{
    std::string localAddress;
    if (AT_HOME)
    {
        localAddress = HOME_ADDRESS;
    }
    else
    {
        localAddress = LAPTOP;
    }

    nexilis::Log::startConsoleDebugging();

    nexilis::ClientAPI::ServerData serverData("salasana");
    serverData.setInetUDP(localAddress, 54200);
    serverData.setInetTCP(localAddress, 54300);
    serverData.setBoostTCP(localAddress, 12345);
    serverData.setBoostUDP(localAddress, 12346);
    serverData.setUnixStreamServerPath("/tmp/nexilis");
    serverData.setUnixDgramServerPath("/tmp/nexilis_dgram");

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
    //auto unixClient = protocolManager.createProtocol<nexilis::af_unix::sock_dgram::Client>();
    /*
        Startup code for the DGRAM unix sockets
    */

    // Boost TCP
    auto boostTCP = protocolManager.createProtocol<nexilis::boost::TCPClient>(api);
    boostTCP.start();
    boostTCP.sendMessage(serverData.getPassword());
    api.waitUntilBoostTCPReady();
    std::cout << "Boost TCP connection ready" << std::endl;
    auto id = nexilis::Packet::Get::clientId(api);
    boostTCP.sendMessage(id);
    std::cout << "BOOST TCP DONE!" << std::endl;

    // Boost UDP
    /*
    std::cout << "Starting boost UDP" << std::endl;
    auto boostUDP = protocolManager.createProtocol<nexilis::boost::UDPClient>(api);
    boostUDP.start();
    boostUDP.sendMessage(serverData.getPassword());
    api.waitUntilInetUDPReady();
    */


    /*
    // af_inet TCP
    auto inetTCP = protocolManager.createProtocol<nexilis::af_inet::TCPClient>(api);
    inetTCP.start();
    inetTCP.sendMessage(serverData.getPassword());
    api.waitUntilInetTCPReady();
    inetTCP.sendMessage(id);
    */

    std::cout << "Client api completed!" << std::endl;

    while(true){}
    return 0;
}

