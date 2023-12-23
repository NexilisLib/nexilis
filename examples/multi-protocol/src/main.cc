#include <nexilis/af_inet/udp_server.hh>
#include <nexilis/websocket/websocket.hh>
#include <nexilis/log.hh>
#include <nexilis/protocol_manager.hh>

int main()
{
    nexilis::ProtocolManager manager;

    nexilis::Log::startConsoleLogging();

    // Create a websocket server.
    auto websocketServer = manager.addProtocol<nexilis::Websocket>();

    // These statements are most likely not required.
    websocketServer.setOpenHandler([](){});
    websocketServer.setCloseHandler([](){});

    auto udpServer = manager.addProtocol<nexilis::UDPServer>();

    // Run websocket server and udp server at the same time.

    std::thread t1([&websocketServer]()
    {
        websocketServer.start();
    });

    std::thread t2([&udpServer]()
    {
        udpServer.start();
    });

    t1.join();
    t2.join();

    return 0;
}

