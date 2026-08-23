#include <nexilis/logger/log.hh>
#include <nexilis/protocol_manager.hh>
#include <nexilis/util.hh>

#include <nexilis/client/client_api.hh>
#include <nexilis/client/packet.hh>

#include <nexilis/client/protocol/af_unix/stream_client.hh>

#include <iostream>

nexilis::client::ClientConfig getServerData(const std::string& address)
{
    nexilis::client::ClientConfig serverData;
    serverData.setPassword("salasana");
    serverData.setUnixStreamServerPath(address);
    serverData.setMode(nexilis::server::AuthenticationMode::password_protected);
    return serverData;
}

int main()
{
    nexilis::Log::startConsoleDebugging();
    nexilis::ProtocolManager protocol_manager;

    auto server_data = getServerData("/tmp/nexilis/stream");
    auto client_api = nexilis::client::ClientAPI(server_data);
    auto unix_stream_client = protocol_manager.createProtocol<nexilis::client::af_unix::StreamClient>(client_api);

    unix_stream_client.start();
    std::cout << "Nexilisclient start called" << std::endl;

    // Wait until the server has authenticated this client.
    client_api.waitUntilUnixStreamReady();

    unix_stream_client.stop();
}
