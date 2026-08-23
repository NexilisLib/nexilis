#include <nexilis/logger/log.hh>
#include <nexilis/protocol_manager.hh>

#include <nexilis/server/protocol/af_unix/stream_server.hh>
#include <nexilis/server/runtime.hh>

#include <chrono>
#include <filesystem>
#include <functional>
#include <iostream>
#include <thread>

int main()
{
    nexilis::Log::startConsoleDebugging();
    nexilis::ProtocolManager protocol_manager;

    // Server.
    nexilis::server::ServerConfig settings;
    settings.setMode(nexilis::server::AuthenticationMode::password_protected);
    settings.setPassphrase("salasana");
    settings.setRootPassword("root");

    std::filesystem::create_directories("/tmp/nexilis");
    auto unix_stream_server = protocol_manager.createProtocol<nexilis::server::af_unix::StreamServer>(settings, "/tmp/nexilis/stream");

    unix_stream_server.start();

    auto condition = [](size_t)
    { return true; };
    auto f = std::function<bool()>([]()
                                   {
        std::cout << "Updating server" << std::endl;
        return true; });

    // Block the main thread and keep serving until interrupted.
    nexilis::server::runtime(condition, f, 1);
}
