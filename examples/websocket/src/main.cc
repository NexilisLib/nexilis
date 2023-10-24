#include <nexilis/protocol_manager.hh>

#include <nexilis/websocket/websocketpp.hh>

int main()
{
    nexilis::ProtocolManager manager;

    nexilis::Log::startConsoleLogging(nexilis::LogLevel::DEBUG);

    auto websocketServer = manager.addProtocol<nexilis::Websocketpp>();

    websocketServer.start();

    return 0;
}
