#include <gtest/gtest.h>

#include <nexilis/client/packet.hh>
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
        nexilis::Log::startConsoleDebugging();

        settings.setMode(nexilis::server::AuthenticationMode::password_protected);
        settings.setPassphrase("salasana");
        settings.setRootPassword("root");

        server = std::make_shared<nexilis::server::nxboost::TCPServer>(
                protocol_manager.createProtocol<nexilis::server::nxboost::TCPServer>(settings));
        server->start();

        std::this_thread::sleep_for(std::chrono::seconds(1));

        nexilis::client::ClientConfig server_data;
        server_data.setPassword("salasana");
        server_data.setBoostTCPAddress("127.0.0.1");
        server_data.setMode(nexilis::server::AuthenticationMode::password_protected);

        api = std::make_unique<nexilis::client::ClientAPI>(server_data);
        client = std::make_shared<nexilis::client::nxboost::TCPClient>(
                protocol_manager.createProtocol<nexilis::client::nxboost::TCPClient>(*api));
    }

    void TearDown() override
    {
        if (client)
        {
            client->stop();
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
        if (server)
        {
            server->stop();
        }
        nexilis::server::ClientStorage::clear();
        nexilis::Log::stopLogging();
    }

    void startClientAndConnect()
    {
        client->start();

        const auto timeout = std::chrono::steady_clock::now() + std::chrono::seconds(10);
        while (server->activeConnectionsCount() == 0 && std::chrono::steady_clock::now() < timeout)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        // Wait extra time for port switch to complete.
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }

    nexilis::ProtocolManager protocol_manager;
    nexilis::server::ServerConfig settings;
    std::unique_ptr<nexilis::client::ClientAPI> api;
    std::shared_ptr<nexilis::server::nxboost::TCPServer> server;
    std::shared_ptr<nexilis::client::nxboost::TCPClient> client;
};

TEST_F(BoostTCPTest, BoostTCPSuccessfulConnection)
{
    client->start();

    std::this_thread::sleep_for(std::chrono::seconds(5));

    EXPECT_TRUE(client->isConnected());
    EXPECT_TRUE(server->hasActiveConnections());
    EXPECT_EQ(server->activeConnectionsCount(), 1);
}

TEST_F(BoostTCPTest, SendMessageAsyncReturnsReadyFuture)
{
    startClientAndConnect();

    auto payload = nexilis::client::Packet::Room::Management::create(*api, "async_room");
    auto future = client->sendMessageAsync(payload);

    auto status = future.wait_for(std::chrono::seconds(10));
    EXPECT_EQ(status, std::future_status::ready);

    // The future should be fulfilled (either value or exception from port switch).
    // Either way, get() should not hang.
    try
    {
        future.get();
    }
    catch (const std::exception& e)
    {
        // Acceptable if the port switch caused the write to fail.
        nexilis::Log::debug("SendMessageAsync threw (expected during port switch): ", e.what());
    }
}

TEST_F(BoostTCPTest, SendMessageAsyncReturnsReadyFutureOnStoppedClient)
{
    auto payload = nexilis::client::Packet::Room::Management::create(*api, "test");
    auto future = client->sendMessageAsync(payload);

    auto status = future.wait_for(std::chrono::seconds(5));
    EXPECT_EQ(status, std::future_status::ready);

    EXPECT_THROW(future.get(), std::runtime_error);
}

TEST_F(BoostTCPTest, SendMessageAsyncReturnsReadyFutureOnClosedSocket)
{
    startClientAndConnect();

    client->stop();

    auto payload = nexilis::client::Packet::Room::Management::create(*api, "test");
    auto future = client->sendMessageAsync(payload);

    auto status = future.wait_for(std::chrono::seconds(5));
    EXPECT_EQ(status, std::future_status::ready);

    EXPECT_THROW(future.get(), std::runtime_error);
}

TEST_F(BoostTCPTest, SendMessageAsyncDeliversMessage)
{
    startClientAndConnect();

    EXPECT_EQ(api->getActiveRooms().size(), 0);
    auto payload = nexilis::client::Packet::Room::Management::create(*api, "async_room");
    auto future = client->sendMessageAsync(payload);

    auto status = future.wait_for(std::chrono::seconds(10));
    ASSERT_EQ(status, std::future_status::ready);

    try
    {
        future.get();
    }
    catch (const std::exception& e)
    {
        nexilis::Log::debug("SendMessageAsync threw: ", e.what());
    }

    // Wait for server to process the message and create the room.
    const auto room_timeout = std::chrono::steady_clock::now() + std::chrono::seconds(10);
    while (api->getActiveRooms().size() == 0 && std::chrono::steady_clock::now() < room_timeout)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    EXPECT_EQ(api->getActiveRooms().size(), 1);
}
