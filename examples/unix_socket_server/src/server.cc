#include <nexilis/protocol_manager.hh>
#include <nexilis/logger/log_level.hh>
#include <nexilis/af_unix/unix_socket_server.hh>
#include <nexilis/log.hh>
#include <nexilis/server_manager.hh>

int main()
{
    nexilis::Log::startConsoleLogging();
    nexilis::Log::setLevel(nexilis::LogLevel::DEBUG);

    nexilis::ProtocolManager manager;

    auto server = manager.addProtocol<nexilis::af_unix::UnixSocketServer>("/tmp/nexilis");

    server.start();

    return 0;
}
