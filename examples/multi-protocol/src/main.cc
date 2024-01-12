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
    auth.setRootPassword("salasana");
    auth.setCommonPassword("common");

    nexilis::ServerManager serverManager;
    serverManager.setAuthentication(auth);

    nexilis::ProtocolManager protocolManager;

    // Create a websocket server.
    auto websocketServer = protocolManager.addProtocol<nexilis::Websocket>();

    websocketServer.setOpenHandler([]()
                                   { std::cout << "Open handler!" << std::endl; });

    websocketServer.setCloseHandler([]()
                                    { std::cout << "Close handler" << std::endl; });

    auto udpServer = protocolManager.addProtocol<nexilis::AfInetUdpServer>();


    // Run websocket server and udp server at the same time.
    std::thread t1([&websocketServer]()
                   { websocketServer.start(); });

    std::thread t2([&udpServer]()
                   { udpServer.start(); });

    t1.join();
    t2.join();

    return 0;
}
