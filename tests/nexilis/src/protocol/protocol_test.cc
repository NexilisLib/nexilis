#include <gtest/gtest.h>

#include <nexilis/client/packet.hh>
#include <nexilis/client/protocol/nxboost/tcp_client.hh>
#include <nexilis/client/protocol/nxboost/udp_client.hh>
#include <nexilis/logger/log.hh>
#include <nexilis/protocol_manager.hh>
#include <nexilis/room_data.hh>
#include <nexilis/server/client_storage.hh>
#include <nexilis/server/protocol/nxboost/tcp_server.hh>
#include <nexilis/server/protocol/nxboost/udp_server.hh>
#include <nexilis/server/room_storage.hh>

static nexilis::ProtocolManager protocol_manager;

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
        std::string rnd = nexilis::Util::getRandomString(10);
        server_data.setUserName("user_" + rnd);
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
        this->server_data.setBoostTCP("127.0.0.1");
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
    EXPECT_TRUE(client->isConnected());
}

TEST_F(BasicBoostTCPTest, ProtocolTestBoostTCPHasActiveConnections)
{
    this->clientStart();
    EXPECT_TRUE(server->hasActiveConnections());
}

TEST_F(BasicBoostTCPTest, ProtocolTestBoostTCPActiveConnectionsCountFromOne)
{
    this->clientStart();
    EXPECT_EQ(server->activeConnectionsCount(), 1);
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
    EXPECT_TRUE(client->isConnected());
    EXPECT_TRUE(server->hasActiveConnections());
    EXPECT_EQ(server->activeConnectionsCount(), 1);
}

TEST_F(RoomBoostTCP2DTest, ProtocolTestBoostTCPRoomInfoRooms)
{
    this->clientStart();

    std::promise<void> promise;
    std::future<void> future = promise.get_future();
    this->client->sendMessage(
            nexilis::client::Packet::Info::rooms(),
            this->api->waitUntilRoomsCreated(promise));
    future.wait();
}
