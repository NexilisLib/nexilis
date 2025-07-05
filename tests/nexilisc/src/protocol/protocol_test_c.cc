#include <gtest/gtest.h>

#include <nexilisc/logger/log_c.h>
#include <nexilisc/nx_data_c.h>
#include <nexilisc/protocol_manager_c.h>
#include <nexilisc/server/settings_c.h>

#include <nexilisc/client/protocol/boost_tcp_client_c.h>
#include <nexilisc/server/protocol/boost_tcp_server_c.h>

// TODO C API
#include <nexilis/server/room_storage.hh>

static nexilis_ProtocolManagerC* protocol_manager = nullptr;

template <typename Server, typename Client>
class ProtocolTestC : public ::testing::Test
{
protected:
    virtual void setup() = 0;
    virtual void defaultSetup() = 0;

    void SetUp() override
    {
        nexilis_log_start_console_debugging();
        protocol_manager = nexilis_protocol_manager_create();
        defaultSetup();
    }

    void TearDown() override
    {
        if (client)
        {
            stop_client(client);
            destroy_client(client);
        }

        sleep(1);

        if (server)
        {
            stop_server(server);
            destroy_server(server);
        }

        if (client_api)
        {
            nexilis_client_api_destroy(client_api);
        }

        if (server_settings)
        {
            nexilis_settings_destroy(server_settings);
        }

        if (server_data)
        {
            nexilis_server_data_destroy(server_data);
        }

        nexilis_protocol_manager_destroy(protocol_manager);
    }

    void createSettings()
    {
        server_settings = nexilis_settings_create();
        nexilis_settings_set_mode(server_settings, AUTHENTICATION_MODE_PASSWORD_PROTECTED);
        nexilis_settings_set_passphrase(server_settings, "salasana");
        nexilis_settings_set_root_password(server_settings, "root");
    }

    void createServer()
    {
        server = create_server_func(protocol_manager, server_settings);
        sleep(1);
    }

    void serverStart()
    {
        start_server(server);
        sleep(1);
    }

    void stopClient(Client* client)
    {
        stop_client(client);
        sleep(1);
    }

    void destroyClient(Client* client)
    {
        destroy_client(client);
        sleep(1);
    }

    void destroyServer(Server* server)
    {
        destroy_server(server);
        sleep(1);
    }

    void createDefaultServerData()
    {
        server_data = nexilis_server_data_create();
        nexilis_server_data_set_password(server_data, "salasana");

        char rnd[11];
        generateRandomString(rnd, 10);
        char username[20];
        snprintf(username, sizeof(username), "user_%s", rnd);
        nexilis_server_data_set_username(server_data, username);
    }

    void createClientAPI()
    {
        client_api = nexilis_client_api_create(server_data);
    }

    void createClient()
    {
        client = create_client_func(protocol_manager, client_api);
    }

    void clientStart()
    {
        start_client(client);
        sleep(1);
    }

    void setAddress()
    {
        set_address(this->server_data, "127.0.0.1");
    }

    void sendMessage(nx_data_c* data)
    {
        send_message(client, data->data->data(), data->data->size());
    }

    // Function pointers for polymorphic behavior.
    Server* (*create_server_func)(nexilis_ProtocolManagerC*, nexilis_server_SettingsC*);
    void (*start_server)(Server*);
    void (*stop_server)(Server*);
    void (*destroy_server)(Server*);

    Client* (*create_client_func)(nexilis_ProtocolManagerC*, nexilis_ClientAPI*);
    void (*start_client)(Client*);
    void (*stop_client)(Client*);
    void (*destroy_client)(Client*);
    bool (*is_connected)(Client*);
    void (*set_address)(nexilis_ServerData*, const char*);
    void (*send_message)(Client*, const uint8_t*, size_t);

    Server* server = nullptr;
    Client* client = nullptr;
    nexilis_server_SettingsC* server_settings = nullptr;
    nexilis_ServerData* server_data = nullptr;
    nexilis_ClientAPI* client_api = nullptr;

private:
    void generateRandomString(char* buf, size_t length)
    {
        static const char alphanum[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
        for (size_t i = 0; i < length; ++i)
        {
            buf[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
        }
        buf[length] = '\0';
    }
};

template <typename Server, typename Client>
class ProtocolTestObjectC : public ProtocolTestC<Server, Client>
{
protected:
    void defaultSetup() override
    {
        this->setup();
        this->createSettings();
        this->createServer();
        this->serverStart();
        this->createDefaultServerData();
        this->setAddress();
        this->createClientAPI();
        this->createClient();
    }
};

// Boost TCP specialization.
template <typename Server, typename Client>
class ProtocolTestBoostTCPC : public ProtocolTestObjectC<Server, Client>
{
protected:
    void setup() override
    {
        this->create_server_func = nexilis_create_boost_tcp_server;
        this->start_server = nexilis_boost_tcp_server_start;
        this->stop_server = nexilis_boost_tcp_server_stop;
        this->destroy_server = nexilis_boost_tcp_server_destroy;

        this->create_client_func = nexilis_boost_tcp_client_create;
        this->start_client = nexilis_boost_tcp_client_start;
        this->stop_client = nexilis_boost_tcp_client_stop;
        this->destroy_client = nexilis_boost_tcp_client_destroy;
        this->is_connected = nexilis_boost_tcp_client_is_connected;
        this->set_address = nexilis_server_data_set_boost_tcp_address;
        this->send_message = nexilis_boost_tcp_client_send_message;
    }
};

using BoostTCPC = ProtocolTestBoostTCPC<nexilis_BoostTCPServer, nexilis_BoostTCPClient>;

TEST_F(BoostTCPC, ProtocolTestBoostTCPClientConnected)
{
    this->clientStart();
    EXPECT_TRUE(this->is_connected(this->client));
}

template <typename ProtocolTestType, nexilis::RoomData::Context RoomContext>
class ProtocolRoomTestC : public ProtocolTestType
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
        this->setAddress();
        this->createClientAPI();
        this->createClient();
    }
};

using RoomBoostTCP2DTestC = ProtocolRoomTestC<BoostTCPC, nexilis::RoomData::Context::_2D>;

TEST_F(RoomBoostTCP2DTestC, ProtocolTestBoostTCPClientConnected)
{
    this->clientStart();
    EXPECT_TRUE(this->is_connected(this->client));
}
