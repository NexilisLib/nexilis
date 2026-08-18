#include <gtest/gtest.h>

#include <nexilisc/client/client_config_c.h>
#include <nexilisc/logger/log_c.h>
#include <nexilisc/nx_data_c.h>
#include <nexilisc/protocol_manager_c.h>
#include <nexilisc/server/server_config_c.h>

#include <nexilisc/client/protocol/boost_tcp_client_c.h>
#include <nexilisc/server/protocol/boost_tcp_server_c.h>

#include <nexilisc/server/room_storage_c.h>

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
            nexilis_server_config_destroy(server_settings);
        }

        if (server_data)
        {
            nexilis_client_config_destroy(server_data);
        }

        nexilis_protocol_manager_destroy(protocol_manager);
    }

    void createSettings()
    {
        server_settings = nexilis_server_config_create();
        nexilis_server_config_set_mode(server_settings, AUTHENTICATION_MODE_PASSWORD_PROTECTED);
        nexilis_server_config_set_passphrase(server_settings, "salasana");
        nexilis_server_config_set_root_password(server_settings, "root");
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
        server_data = nexilis_client_config_create();
        nexilis_client_config_set_password(server_data, "salasana");
        nexilis_client_config_set_mode(server_data, AUTHENTICATION_MODE_PASSWORD_PROTECTED);
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
    Server* (*create_server_func)(nexilis_ProtocolManagerC*, nexilis_server_ConfigC*);
    void (*start_server)(Server*);
    void (*stop_server)(Server*);
    void (*destroy_server)(Server*);

    Client* (*create_client_func)(nexilis_ProtocolManagerC*, nexilis_ClientAPI*);
    void (*start_client)(Client*);
    void (*stop_client)(Client*);
    void (*destroy_client)(Client*);
    bool (*is_connected)(Client*);
    void (*set_address)(nexilis_ClientConfigC*, const char*);
    void (*send_message)(Client*, const uint8_t*, size_t);

    Server* server = nullptr;
    Client* client = nullptr;
    nexilis_server_ConfigC* server_settings = nullptr;
    nexilis_ClientConfigC* server_data = nullptr;
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
        this->set_address = nexilis_client_config_set_boost_tcp_address;
        this->send_message = nexilis_boost_tcp_client_send_message;
    }
};

using BoostTCPC = ProtocolTestBoostTCPC<nexilis_BoostTCPServer, nexilis_BoostTCPClient>;

TEST_F(BoostTCPC, ProtocolTestBoostTCPClientConnected)
{
    this->clientStart();
    EXPECT_TRUE(this->is_connected(this->client));
}

template <typename ProtocolTestType, nexilis_RoomContext RoomContext>
class ProtocolRoomTestC : public ProtocolTestType
{
protected:
    void defaultSetup() override
    {
        this->setup();
        this->createSettings();
        this->createServer();

        nexilis_RoomData* room_data = nexilis_room_data_create(0, "RoomTestRoom", RoomContext, 10);
        uint64_t id = nexilis_room_data_get_id(room_data);
        nexilis_server_room_storage_add(room_data);
        EXPECT_TRUE(nexilis_server_room_storage_contains(id));
        auto* room = nexilis_server_room_storage_get_room_by_id(id);
        EXPECT_TRUE(room != nullptr);
        delete room;

        this->serverStart();
        this->createDefaultServerData();
        this->setAddress();
        this->createClientAPI();
        this->createClient();
    }
};

using RoomBoostTCP2DTestC = ProtocolRoomTestC<BoostTCPC, ROOM_CONTEXT_2D>;

TEST_F(RoomBoostTCP2DTestC, ProtocolTestBoostTCPClientConnected)
{
    this->clientStart();
    EXPECT_TRUE(this->is_connected(this->client));
}

class RoomStorageTestC : public ::testing::Test
{
protected:
    void SetUp() override
    {
        nexilis_server_room_storage_clear();
    }

    void TearDown() override
    {
        nexilis_server_room_storage_clear();
    }
};

TEST_F(RoomStorageTestC, AddRoom)
{
    nexilis_RoomData* data = nexilis_room_data_create(0, "TestRoom", ROOM_CONTEXT_2D, 10);
    uint64_t id = nexilis_room_data_get_id(data);

    nexilis_server_room_storage_add(data);
    EXPECT_TRUE(nexilis_server_room_storage_contains(id));
    EXPECT_EQ(nexilis_server_room_storage_get_all_rooms_count(), 1u);
}

TEST_F(RoomStorageTestC, ContainsReturnsFalseForUnknownId)
{
    EXPECT_FALSE(nexilis_server_room_storage_contains(999999));
}

TEST_F(RoomStorageTestC, GetRoomByIdReturnsNullptrForUnknownId)
{
    nexilis_ServerRoom* room = nexilis_server_room_storage_get_room_by_id(999999);
    EXPECT_EQ(room, nullptr);
}

TEST_F(RoomStorageTestC, GetRoomByIdReturnsRoom)
{
    nexilis_RoomData* data = nexilis_room_data_create(10, "LookupRoom", ROOM_CONTEXT_3D, 10);
    uint64_t id = nexilis_room_data_get_id(data);
    nexilis_server_room_storage_add(data);

    nexilis_ServerRoom* room = nexilis_server_room_storage_get_room_by_id(id);
    ASSERT_NE(room, nullptr);
    delete room;
}

TEST_F(RoomStorageTestC, GetAllRoomsCount)
{
    EXPECT_EQ(nexilis_server_room_storage_get_all_rooms_count(), 0u);

    nexilis_server_room_storage_add(nexilis_room_data_create(0, "Room1", ROOM_CONTEXT_2D, 10));
    nexilis_server_room_storage_add(nexilis_room_data_create(0, "Room2", ROOM_CONTEXT_3D, 10));
    EXPECT_EQ(nexilis_server_room_storage_get_all_rooms_count(), 2u);
}

TEST_F(RoomStorageTestC, GetRoomAtReturnsRoom)
{
    nexilis_server_room_storage_add(nexilis_room_data_create(0, "IndexedRoom", ROOM_CONTEXT_2D, 10));

    nexilis_ServerRoom* room = nexilis_server_room_storage_get_room_at(0);
    ASSERT_NE(room, nullptr);
    delete room;
}

TEST_F(RoomStorageTestC, GetRoomAtReturnsNullptrForOutOfBounds)
{
    nexilis_ServerRoom* room = nexilis_server_room_storage_get_room_at(0);
    EXPECT_EQ(room, nullptr);
}

TEST_F(RoomStorageTestC, ClearRemovesAllRooms)
{
    nexilis_server_room_storage_add(nexilis_room_data_create(0, "Room1", ROOM_CONTEXT_2D, 10));
    nexilis_server_room_storage_add(nexilis_room_data_create(0, "Room2", ROOM_CONTEXT_3D, 10));
    EXPECT_EQ(nexilis_server_room_storage_get_all_rooms_count(), 2u);

    nexilis_server_room_storage_clear();
    EXPECT_EQ(nexilis_server_room_storage_get_all_rooms_count(), 0u);
}

TEST_F(RoomStorageTestC, JoinAndLeaveRoom)
{
    nexilis_RoomData* data = nexilis_room_data_create(0, "JoinRoom", ROOM_CONTEXT_2D, 10);
    uint64_t id = nexilis_room_data_get_id(data);
    nexilis_server_room_storage_add(data);

    nexilis_ServerRoom* room = nexilis_server_room_storage_get_room_by_id(id);
    ASSERT_NE(room, nullptr);

    nexilis_server_room_join(room, 100);
    EXPECT_TRUE(nexilis_server_room_contains(room, 100));
    EXPECT_EQ(nexilis_server_room_get_client_count(room), 1u);

    nexilis_server_room_join(room, 200);
    EXPECT_EQ(nexilis_server_room_get_client_count(room), 2u);

    nexilis_server_room_leave(room, 100);
    EXPECT_FALSE(nexilis_server_room_contains(room, 100));
    EXPECT_EQ(nexilis_server_room_get_client_count(room), 1u);

    delete room;
}

TEST_F(RoomStorageTestC, RoomContainsReturnsFalseForAbsentUser)
{
    nexilis_RoomData* data = nexilis_room_data_create(0, "ContainsRoom", ROOM_CONTEXT_2D, 10);
    uint64_t id = nexilis_room_data_get_id(data);
    nexilis_server_room_storage_add(data);

    nexilis_ServerRoom* room = nexilis_server_room_storage_get_room_by_id(id);
    ASSERT_NE(room, nullptr);

    EXPECT_FALSE(nexilis_server_room_contains(room, 999));

    delete room;
}
