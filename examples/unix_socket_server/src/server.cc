#include <nexilis/protocol_manager.hh>
#include <nexilis/logger/log_level.hh>
#include <nexilis/af_unix/unix_socket_server.hh>
#include <nexilis/log.hh>
#include <nexilis/server_manager.hh>
#include <nexilis/authentication.hh>

int main()
{
    nexilis::Log::startConsoleLogging();
    nexilis::Log::setLevel(nexilis::LogLevel::DEBUG);

    nexilis::Authentication auth;
    auth.setMode(nexilis::Authentication::Mode::passwordProtected);
    auth.setCommonPassword("salasana");

    nexilis::ServerManager serverManager;
    serverManager.setAuthentication(auth);

    nexilis::ProtocolManager manager;

    auto server = manager.createProtocol<nexilis::af_unix::UnixSocketServer>("/tmp/nexilis");

    server.start();

    return 0;
}
