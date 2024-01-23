#include <nexilis/authentication.hh>
#include <nexilis/log.hh>
#include <nexilis/protocol_manager.hh>
#include <nexilis/server_manager.hh>
#include <nexilis/af_unix/sock_stream/server.hh>

int main()
{
    //nexilis::Log::startConsoleLogging();
    //nexilis::Log::setLevel(nexilis::LogLevel::DEBUG);

    nexilis::Authentication auth;
    auth.setMode(nexilis::Authentication::Mode::passwordProtected);
    auth.setCommonPassword("salasana");

    nexilis::ServerManager serverManager;
    serverManager.setAuthentication(auth);

    nexilis::ProtocolManager protocolManager;

    auto server = protocolManager.createProtocol<nexilis::af_unix::sock_stream::Server>("/tmp/nexilis");

    server.start();

    return 0;
}
