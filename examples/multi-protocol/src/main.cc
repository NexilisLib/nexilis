#include <nexilis/af_inet/udp_server.hh>
#include <nexilis/log.hh>
#include <nexilis/protocol_manager.hh>
#include <nexilis/websocket/websocket.hh>

int main()
{
    nexilis::Log::startConsoleLogging();

    nexilis::ProtocolManager manager;

    // Create a websocket server.
    auto websocketServer = manager.addProtocol<nexilis::Websocket>();

    // These statements are most likely not required.
    websocketServer.setOpenHandler([]()
                                   { std::cout << "Open handler!" << std::endl; });

    websocketServer.setCloseHandler([]()
                                    { std::cout << "Close handler" << std::endl; });

    auto udpServer = manager.addProtocol<nexilis::AfInetUdpServer>();

    // Run websocket server and udp server at the same time.
    std::thread t1([&websocketServer]()
                   { websocketServer.start(); });

    std::thread t2([&udpServer]()
                   { udpServer.start(); });

    t1.join();
    t2.join();

    return 0;
}
