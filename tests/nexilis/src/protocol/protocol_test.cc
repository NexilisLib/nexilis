#include <gtest/gtest.h>

#include <nexilis/client/packet.hh>
#include <nexilis/client/protocol/nxboost/tcp_client.hh>
#include <nexilis/client/protocol/nxboost/udp_client.hh>
#include <nexilis/environment.hh>
#include <nexilis/logger/log.hh>
#include <nexilis/protocol_manager.hh>
#include <nexilis/room_data.hh>
#include <nexilis/server/client_storage.hh>
#include <nexilis/server/protocol/nxboost/tcp_server.hh>
#include <nexilis/server/protocol/nxboost/udp_server.hh>
#include <nexilis/server/room_storage.hh>

static nexilis::ProtocolManager protocol_manager;

void waitFor(uint32_t seconds, bool condition)
{
    const auto timeout = std::chrono::steady_clock::now() + std::chrono::seconds(seconds);
    while (!condition && std::chrono::steady_clock::now() < timeout)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

template <typename Client, typename Server>
void waitRoomInfo(std::shared_ptr<Client>& client, std::shared_ptr<Server>& server, std::unique_ptr<nexilis::client::ClientAPI>& api)
{
    const auto environment = nexilis::detectRuntimeType();
    const bool is_ci = environment == nexilis::EnvironmentType::ci;
    const auto send_timeout = is_ci ? std::chrono::seconds(10) : std::chrono::seconds(3);

    std::promise<void> promise;
    auto future = promise.get_future();
    client->sendMessage(
            nexilis::client::Packet::Get::Info::rooms(),
            api->waitUntilRoomsCreated(promise));

    auto status = future.wait_for(send_timeout);
    if (status != std::future_status::ready)
    {
        std::ostringstream oss;
        oss << "Test timeout - Final state:\n"
            << "  Client connected: " << client->isConnected() << "\n"
            << "  Server connections: " << server->activeConnectionsCount() << "\n"
            << "  Active rooms: " << api->getActiveRooms().size() << "\n"
            << "  Protocol status: " << client->getProtocolStatusString() << std::endl
            << "  CI Environment: " << (is_ci ? "Yes" : "No");
        std::cerr << oss.str() << std::endl;
    }
}

template <typename Server, typename Client>
class ProtocolTest : public ::testing::Test
{
protected:
    virtual void setup() = 0;
    virtual void defaultSetup() = 0;

    void SetUp() override
    {
        nexilis::Log::startConsoleDebugging();
        defaultSetup();
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
        nexilis::Log::stopLogging();
        nexilis::server::ClientStorage::clear();
        nexilis::server::RoomStorage::clear();
    }

    void createSettings()
    {
        settings.setMode(nexilis::server::AuthenticationMode::password_protected);
        settings.setPassphrase("salasana");
        settings.setRootPassword("root");
    }

    void createServer()
    {
        server = std::make_shared<Server>(protocol_manager.createProtocol<Server>(settings));
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    void serverStart()
    {
        server->start();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    void createDefaultServerData()
    {
        server_data.setPassword("salasana");
    }

    void createClientAPI()
    {
        api = std::make_unique<nexilis::client::ClientAPI>(server_data);
    }

    void createClient()
    {
        client = std::make_shared<Client>(protocol_manager.createProtocol<Client>(*api));
    }

    void clientStart()
    {
        client->start();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    std::shared_ptr<Server> server;
    std::shared_ptr<Client> client;

    nexilis::server::Settings settings;
    nexilis::client::ServerData server_data;
    std::unique_ptr<nexilis::client::ClientAPI> api;
};

template <typename Server, typename Client>
class ProtocolTestBoostTCP : public ProtocolTest<Server, Client>
{
protected:
    void setup() override
    {
        this->server_data.setBoostTCPAddress("127.0.0.1");
    }
};

template <typename Server, typename Client>
class ProtocolTestBoostUDP : public ProtocolTest<Server, Client>
{
protected:
    void setup() override
    {
        this->server_data.setBoostUDP("127.0.0.1");
    }
};

template <typename ProtocolTestType>
class ProtocolBasicTest : public ProtocolTestType
{
protected:
    void defaultSetup() override
    {
        this->setup();
        this->createSettings();
        this->createServer();
        this->serverStart();
        this->createDefaultServerData();
        this->createClientAPI();
        this->createClient();
    }
};

using BasicBoostTCPTest = ProtocolBasicTest<ProtocolTestBoostTCP<
        nexilis::server::nxboost::TCPServer, nexilis::client::nxboost::TCPClient>>;

TEST_F(BasicBoostTCPTest, ProtocolTestBoostTCPClientConnected)
{
    this->clientStart();

    waitFor(5, client->isConnected());
    EXPECT_TRUE(client->isConnected());
}

TEST_F(BasicBoostTCPTest, ProtocolTestBoostTCPHasActiveConnections)
{
    this->clientStart();

    waitFor(5, server->hasActiveConnections());
    EXPECT_TRUE(server->hasActiveConnections());
}

TEST_F(BasicBoostTCPTest, ProtocolTestBoostTCPActiveConnectionsCountFromOne)
{
    this->clientStart();

    waitFor(5, server->activeConnectionsCount() == 1);
    EXPECT_EQ(server->activeConnectionsCount(), 1);
}

TEST_F(BasicBoostTCPTest, ProtocolTestBoostTCPCreateRoom)
{
    this->clientStart();

    EXPECT_EQ(api->getActiveRooms().size(), 0);

    auto ctx = nexilis::RoomData::Context::_2D;
    auto create_room = nexilis::client::Packet::Room::Management::create(ctx, "room_mayn");

    this->client->sendMessage(create_room);
    waitFor(5, api->getActiveRooms().size() == 1);

    EXPECT_EQ(api->getActiveRooms().size(), 1);
}

TEST_F(BasicBoostTCPTest, ProtocolTestBoostTCPJoinRoom)
{
    this->clientStart();

    EXPECT_EQ(api->getActiveRooms().size(), 0);

    auto ctx = nexilis::RoomData::Context::_2D;
    // auto user =
    auto create_room = nexilis::client::Packet::Room::Management::create(ctx, "room1");

    this->client->sendMessage(create_room);
    waitFor(5, api->getActiveRooms().size() == 1);

    // auto join_room = nexilis::client::Packet::Room::Management::join(uint64_t roomId)

    // EXPECT_EQ(api->getActiveRooms().size(), 1);
}

using BasicBoostUDPTest = ProtocolTestBoostTCP<nexilis::server::nxboost::UDPServer,
                                               nexilis::client::nxboost::UDPClient>;

template <typename ProtocolTestType, nexilis::RoomData::Context RoomContext>
class ProtocolRoomTest : public ProtocolTestType
{
protected:
    void defaultSetup() override
    {
        this->setup();
        this->createSettings();
        this->createServer();

        auto room = nexilis::server::Room(nexilis::RoomData(
                0,
                "RoomTestRoom",
                nexilis::Util::getRandomUint64(),
                RoomContext));
        auto id = room.getId();
        nexilis::server::RoomStorage::add(std::move(room));
        EXPECT_TRUE(nexilis::server::RoomStorage::contains(id));
        EXPECT_TRUE(nexilis::server::RoomStorage::getRoomById(id) != nullptr);

        this->serverStart();
        this->createDefaultServerData();
        this->createClientAPI();
        this->createClient();
    }
};

using RoomBoostTCP2DTest = ProtocolRoomTest<BasicBoostTCPTest, nexilis::RoomData::Context::_2D>;

TEST_F(RoomBoostTCP2DTest, ProtocolTestBoostTCPRoomClientConnected)
{
    this->clientStart();

    waitFor(5, client->isConnected() && server->hasActiveConnections());

    EXPECT_TRUE(client->isConnected());
    EXPECT_TRUE(server->hasActiveConnections());
    EXPECT_EQ(server->activeConnectionsCount(), 1);
}

TEST_F(RoomBoostTCP2DTest, ProtocolTestBoostTCPRoomCreation)
{
    ASSERT_FALSE(client->isConnected());
    ASSERT_FALSE(server->hasActiveConnections());
    this->clientStart();

    waitRoomInfo(this->client, this->server, this->api);
    EXPECT_EQ(api->getActiveRooms().size(), 1);

    this->client->sendMessage(nexilis::client::Packet::Room::Management::create(nexilis::RoomData::Context::_2D, "test"));
    waitRoomInfo(this->client, this->server, this->api);

    EXPECT_EQ(api->getActiveRooms().size(), 2);
}

TEST_F(RoomBoostTCP2DTest, ProtocolTestBoostTCPRoomInfoRooms)
{
    // Fresh state.
    ASSERT_FALSE(client->isConnected());
    ASSERT_FALSE(server->hasActiveConnections());

    // CI-aware timeout settings.
    const auto environment = nexilis::detectRuntimeType();
    const bool is_ci = environment == nexilis::EnvironmentType::ci;
    const auto connection_timeout = is_ci ? std::chrono::seconds(15) : std::chrono::seconds(5);
    const auto send_timeout = is_ci ? std::chrono::seconds(10) : std::chrono::seconds(3);
    const int max_send_attempts = is_ci ? 5 : 3;

    // Start client.
    this->clientStart();

    const auto connect_start = std::chrono::steady_clock::now();
    while (!client->isConnected() || !server->hasActiveConnections())
    {
        if (std::chrono::steady_clock::now() - connect_start > connection_timeout)
        {
            FAIL() << "Connection timeout - Client: " << client->isConnected()
                   << " Server: " << server->hasActiveConnections();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    std::promise<void> promise;
    auto future = promise.get_future();
    bool send_success = false;

    this->client->sendMessage(nexilis::client::Packet::Room::Management::create(nexilis::RoomData::Context::_2D, "test"));

    // Enhanced send with retries.
    for (int attempt = 0; attempt < max_send_attempts && !send_success; ++attempt)
    {
        try
        {
            // Check if we're mid-port-switch.
            if (client->getProtocolStatus() == nexilis::client::ProtocolStatus::switching_ports)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(200 * (attempt + 1)));
                continue;
            }

            this->client->sendMessage(
                    nexilis::client::Packet::Get::Info::rooms(),
                    this->api->waitUntilRoomsCreated(promise));

            send_success = true;
        }
        catch (const std::exception& e)
        {
            if (attempt == max_send_attempts - 1)
            {
                FAIL() << "Message send failed after " << max_send_attempts
                       << " attempts: " << e.what();
            }
            std::cout << "Send attempt " << (attempt + 1) << " failed: " << e.what() << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    // Verify results with CI-extended timeout.
    auto status = future.wait_for(send_timeout);
    if (status != std::future_status::ready)
    {
        std::ostringstream oss;
        oss << "Test timeout - Final state:\n"
            << "  Client connected: " << client->isConnected() << "\n"
            << "  Server connections: " << server->activeConnectionsCount() << "\n"
            << "  Active rooms: " << api->getActiveRooms().size() << "\n"
            << "  Protocol status: " << client->getProtocolStatusString() << std::endl
            << "  CI Environment: " << (is_ci ? "Yes" : "No");
        std::cerr << oss.str() << std::endl;
    }

    EXPECT_EQ(api->getActiveRooms().size(), 2);
    EXPECT_EQ(status, std::future_status::ready);
}
