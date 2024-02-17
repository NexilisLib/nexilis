#include <nexilis/protocol_manager.hh>
#include <nexilis/af_inet/udp_client.hh>
#include <nexilis/af_inet/tcp_client.hh>
#include <nexilis/client_api.hh>
#include <nexilis/boost/tcp_client.hh>
#include <nexilis/af_unix/sock_stream/client.hh>
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

    nexilis::ClientAPI api(serverData);
    nexilis::ProtocolManager protocolManager;

    // TCP
    /*
    auto tcpClient = protocolManager.createProtocol<nexilis::af_inet::TCPClient>(api);

    if (tcpClient.connectToServer())
    {
        const char* message = "salasana";
        tcpClient.send(message, strlen(message));
    }
    */

    /// UDP
    /*
    auto inetUDP = protocolManager.createProtocol<nexilis::af_inet::UDPClient>(api);
    inetUDP.start();
    inetUDP.sendMessage(serverData.getPassword());
    std::this_thread::sleep_for(std::chrono::seconds(1));
    auto id = nexilis::Packet::Get::clientId(api);
    inetUDP.sendMessage(id);
    */

    // Boost TCP
    auto boostTCP = protocolManager.createProtocol<nexilis::boost::TCPClient>(api);
    boostTCP.start();
    boostTCP.sendMessage(serverData.getPassword());
    api.waitUntilBoostTCPReady();
    auto id = nexilis::Packet::Get::clientId(api);
    boostTCP.sendMessage(id);

    /* 
    auto sockStream = protocolManager.createProtocol<nexilis::af_unix::sock_stream::Client>(api);
    sockStream.start();
    */

    while(true){}
    return 0;
}

