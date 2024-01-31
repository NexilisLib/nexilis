#include <nexilis/protocol_manager.hh>
#include <nexilis/af_inet/udp_client.hh>
#include <nexilis/af_inet/tcp_client.hh>
#include <nexilis/client_api/client_api.hh>
#include <nexilis/boost/tcp_client.hh>

int main()
{
    nexilis::Log::startConsoleLogging();
    nexilis::Log::setLevel(nexilis::logger::LogLevel::DEBUG);

    nexilis::ClientAPI::ServerData serverData("salasana");
    serverData.setInetUDP("192.168.1.85", 54200);
    serverData.setInetTCP("192.168.1.85", 54300);

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
    //auto afInet = protocolManager.createProtocol<nexilis::af_inet::UDPClient>(api);
    //afInet.start();

    // Boost TCP
    auto boostTCP = protocolManager.createProtocol<nexilis::boost::TCPClient>("192.168.1.85", "12345");

    boostTCP.start();

    //while (true) {}

    return 0;
}

