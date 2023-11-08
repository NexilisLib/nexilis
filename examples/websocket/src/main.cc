#include <nexilis/log.hh>
#include <nexilis/protocol_manager.hh>

int main()
{
    nexilis::ProtocolManager manager;

    nexilis::Log::startConsoleLogging(nexilis::LogLevel::DEBUG);

    // Create websocketserver object.
    auto websocketServer = manager.addProtocol<nexilis::Websocket>();

    // Set functionality when opening connection.
    websocketServer.setOpenHandler([]()
                                   { std::cout << "Opened a connection!" << std::endl; });

    // Same for closing the connection.
    websocketServer.setCloseHandler([]()
                                    { std::cout << "Closed a connection!" << std::endl; });

    websocketServer.start();

    return 0;
}
