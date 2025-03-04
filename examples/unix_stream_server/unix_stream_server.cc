#include <nexilis/logger/log.hh>
#include <nexilis/protocol_manager.hh>

#include <nexilis/server/protocol/af_unix/stream_server.hh>
#include <nexilis/server/runtime.hh>

#include <iostream>

int main()
{
    nexilis::Log::startConsoleDebugging();
    nexilis::ProtocolManager protocol_manager;

    // Server.
    nexilis::server::Settings settings;
    settings.setMode(nexilis::server::Settings::AuthenticationMode::passwordProtected);
    settings.setPassphrase("salasana");
    settings.setRootPassword("root");

    // TODO automatically create /tmp/nexilis/stream
    auto unix_stream_server = protocol_manager.createProtocol<nexilis::server::af_unix::StreamServer>(settings, "/tmp/nexilis/stream");

    unix_stream_server.start();

    auto condition = [](size_t)
    { return true; };
    auto f = std::function<bool()>([]()
                                   {
        std::cout << "Updating server" << std::endl;
        return true; });

    auto server_runtime = std::thread([&condition, &f]()
                                      { nexilis::server::runtime(condition, f, 1); });

    server_runtime.detach();
}