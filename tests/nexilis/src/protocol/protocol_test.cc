#include <gtest/gtest.h>

#include <nexilis/client/protocol/nxboost/tcp_client.hh>
#include <nexilis/client/protocol/nxboost/udp_client.hh>
#include <nexilis/logger/log.hh>
#include <nexilis/protocol_manager.hh>
#include <nexilis/server/client_storage.hh>
#include <nexilis/server/protocol/nxboost/tcp_server.hh>
#include <nexilis/server/protocol/nxboost/udp_server.hh>

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
        settings.setMode(nexilis::server::Settings::AuthenticationMode::passwordProtected);
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
    nexilis::client::ClientAPI::ServerData server_data;
    std::unique_ptr<nexilis::client::ClientAPI> api;
};

template <typename Server, typename Client>
class ProtocolTestObject : public ProtocolTest<Server, Client>
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

template <typename Server, typename Client>
class ProtocolTestBoostTCP : public ProtocolTestObject<Server, Client>
{
    void setup() override
    {
        this->server_data.setBoostTCP("127.0.0.1");
    }
};

template <typename Server, typename Client>
class ProtocolTestBoostUDP : public ProtocolTestObject<Server, Client>
{
    void setup() override
    {
        this->server_data.setBoostUDP("127.0.0.1");
    }
};

using BoostTCP = ProtocolTestBoostTCP<nexilis::server::nxboost::TCPServer, nexilis::client::nxboost::TCPClient>;

TEST_F(BoostTCP, ProtocolTestBoostTCPClientConnected)
{
    this->clientStart();
    EXPECT_TRUE(client->isConnected());
}

TEST_F(BoostTCP, ProtocolTestBoostTCPHasActiveConnections)
{
    this->clientStart();
    EXPECT_TRUE(server->hasActiveConnections());
}

TEST_F(BoostTCP, ProtocolTestBoostTCPActiveConnectionsCountFromOne)
{
    this->clientStart();
    EXPECT_EQ(server->activeConnectionsCount(), 1);
}

using BoostUDP = ProtocolTestBoostTCP<nexilis::server::nxboost::UDPServer, nexilis::client::nxboost::UDPClient>;
