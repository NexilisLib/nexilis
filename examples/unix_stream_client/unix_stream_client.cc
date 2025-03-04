#include <nexilis/logger/log.hh>
#include <nexilis/protocol_manager.hh>
#include <nexilis/util.hh>

#include <nexilis/client/client_api.hh>
#include <nexilis/client/packet.hh>

#include <nexilis/client/protocol/af_unix/stream_client.hh>

#include <iostream>

nexilis::client::ClientAPI::ServerData getServerData(const std::string& address, const std::string& username)
{
    nexilis::client::ClientAPI::ServerData serverData;
    serverData.setPassword("salasana");
    serverData.setUserName(username);
    serverData.setUnixStreamServerPath(address);
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

    // Convert the passphrase into nexilis format (std::vector<uint8_t>).
    auto message = nexilis::Util::convertToByteVector(server_data.getPassword().c_str(), server_data.getPassword().size());
    unix_stream_client.sendMessage(message);
    std::cout << "Sent password" << std::endl;
    client_api.waitUntilUnixStreamReady();

    // Set username.
    unix_stream_client.sendMessage(nexilis::client::Packet::Set::username(client_name));
}