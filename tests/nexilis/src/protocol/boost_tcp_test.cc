#include <gtest/gtest.h>

#include <nexilis/logger/log.hh>
#include <nexilis/protocol_manager.hh>
#include <nexilis/server/client_storage.hh>

#include <nexilis/client/protocol/nxboost/tcp_client.hh>
#include <nexilis/server/protocol/nxboost/tcp_server.hh>

class BoostTCPTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // Set up the server
        settings.setMode(nexilis::server::Settings::AuthenticationMode::passwordProtected);
        settings.setPassphrase("salasana");
        settings.setRootPassword("root");

        server = std::make_shared<nexilis::server::nxboost::TCPServer>(protocol_manager.createProtocol<nexilis::server::nxboost::TCPServer>(settings));
        server->start();

        std::this_thread::sleep_for(std::chrono::seconds(1));

        // Set up the client
        nexilis::client::ClientAPI::ServerData server_data;
        server_data.setPassword("salasana");
        server_data.setUserName("example_user");
        server_data.setBoostTCP("127.0.0.1");

        api = std::make_unique<nexilis::client::ClientAPI>(server_data);
        client = std::make_shared<nexilis::client::nxboost::TCPClient>(protocol_manager.createProtocol<nexilis::client::nxboost::TCPClient>(*api));
    }

    void TearDown() override
    {
        if (client)
        {
            client->stop();
        }
        if (server)
        {
            server->stop();
        }
    }

    nexilis::ProtocolManager protocol_manager;

    // Nexilis server settings.
    nexilis::server::Settings settings;

    std::unique_ptr<nexilis::client::ClientAPI> api;

    // Nexilis protocols.
    std::shared_ptr<nexilis::server::nxboost::TCPServer> server;
    std::shared_ptr<nexilis::client::nxboost::TCPClient> client;
};

TEST_F(BoostTCPTest, BoostTCPSuccessfulConnection)
{
    nexilis::Log::startConsoleDebugging();

    // Start the client and check if it connects successfully
    client->start();

    // Give the client some time to establish the connection
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Check if the client is connected
    EXPECT_TRUE(client->isConnected());

    // Check if the server has an active connection
    EXPECT_TRUE(server->hasActiveConnections());
    EXPECT_EQ(server->activeConnectionsCount(), 1);

    // Remove client from static storage.
    nexilis::server::ClientStorage::clear();

    nexilis::Log::stopLogging();
}
