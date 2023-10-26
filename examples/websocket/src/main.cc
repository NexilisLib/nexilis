#include <nexilis/protocol_manager.hh>

int main()
{
    nexilis::ProtocolManager manager;

    nexilis::Log::startConsoleLogging(nexilis::LogLevel::DEBUG);

    auto websocketServer = manager.addProtocol<nexilis::Websocket>();

    websocketServer.start();

    return 0;
}
