#include <nexilis/logger/log.hh>
#include <nexilis/protocol_manager.hh>
#include <nexilis/util.hh>

#include <nexilis/client/client_api.hh>
#include <nexilis/client/packet.hh>

#include <nexilis/client/protocol/af_unix/stream_client.hh>

#include <iostream>

nexilis::client::ServerData getServerData(const std::string& address, const std::string& username)
{
    nexilis::client::ServerData serverData;
    serverData.setPassword("salasana");
    serverData.setUserName(username);
    serverData.setUnixStreamServerPath(address);
    serverData.setAuthenticationMode(nexilis::server::AuthenticationMode::password_protected);
    return serverData;
}

std::string randomClientName()
{
    return "client_" + nexilis::Util::getRandomString(5);
}

int main()
{
    nexilis::Log::startConsoleDebugging();
    nexilis::ProtocolManager protocol_manager;

    auto client_name = randomClientName();
    auto server_data = getServerData("/tmp/nexilis/stream", client_name);
    auto client_api = nexilis::client::ClientAPI(server_data);
    auto unix_stream_client = protocol_manager.createProtocol<nexilis::client::af_unix::StreamClient>(client_api);

    unix_stream_client.start();
    std::cout << "Nexilisclient start called" << std::endl;
}
