#include <nexilis/af_inet/udp_server.hh>
#include <nexilis/log.hh>
#include <nexilis/protocol_manager.hh>
#include <nexilis/websocket/websocket.hh>
#include <nexilis/server_manager.hh>
#include <nexilis/af_inet/tcp_server.hh>
#include <nexilis/boost/tcp_server.hh>

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

    // UDP
    //auto udpServer = protocolManager.createProtocol<nexilis::af_inet::UDPServer>();
    //udpServer.start();

    // TCP
    //auto tcpServer = protocolManager.createProtocol<nexilis::af_inet::TCPServer>(54300);
    //tcpServer.start();

    // Boost TCP
    auto tcpServer = protocolManager.createProtocol<nexilis::boost::TCPServer>("12345");
    tcpServer.startListening();

    std::cout << "Started listening" << std::endl;

    if (tcpServer.acceptClient())
    {
        std::cout << "Client accepted" << std::endl;
        while (true)
        {
            std::string buffer;
            if (tcpServer.receiveFromClient(buffer))
            {
                if (!buffer.empty())
                {
                    std::cout << "Received from client: " << buffer << std::endl;
                    tcpServer.sendToClient("moika\n");
                }
                else
                {
                    std::cout << "Received empty message from client" << std::endl;
                    break;
                }
            }
            else
            {
                std::cout << "Not received from client" << std::endl;
            }
        }
    }
    else
    {
        std::cout << "Not accepted" << std::endl;
    }

    return 0;
}
