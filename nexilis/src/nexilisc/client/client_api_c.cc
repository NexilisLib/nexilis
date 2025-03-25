#include <nexilisc/client/client_api_c.h>
#include <nexilisc/room_data_c.h>

struct nexilis_ServerData
{
    nexilis::client::ClientAPI::ServerData* data;
};

struct nexilis_ClientSession
{
    nexilis::client::ClientAPI::ClientSession* session;
};

struct nexilis_Room
{
    nexilis::client::ClientAPI::Room* room;
};

struct nexilis_Communication
{
    nexilis::client::ClientAPI::Room::Communication* communication;
};

nexilis_ServerData* nexilis_server_data_create()
{
    auto server_data = new nexilis_ServerData();
    auto cpp_data = new nexilis::client::ClientAPI::ServerData();
    server_data->data = cpp_data;
    return server_data;
}

void nexilis_server_data_destroy(nexilis_ServerData* server_data)
{
    if (server_data)
    {
        if (server_data->data)
        {
            delete server_data->data;
            server_data->data = nullptr;
        }
        delete server_data;
    }
}

void nexilis_server_data_set_username(nexilis_ServerData* server_data, const char* username)
{
    if (server_data && server_data->data && username)
    {
        server_data->data->setUserName(username);
    }
}

const char* nexilis_server_data_get_username(const nexilis_ServerData* server_data)
{
    if (server_data && server_data->data)
    {
        std::string address = server_data->data->getUsername();
        char* cstr = (char*)malloc(address.size() + 1);
        if (cstr)
        {
            std::strcpy(cstr, address.c_str());
        }
        return cstr;
    }
    return nullptr;
}

void nexilis_server_data_set_password(nexilis_ServerData* server_data, const char* password)
{
    if (server_data && server_data->data && password)
    {
        server_data->data->setPassword(password);
    }
}

const char* nexilis_server_data_get_password(const nexilis_ServerData* server_data)
{
    if (server_data && server_data->data)
    {
        std::string password = server_data->data->getPassword();
        char* cstr = (char*)malloc(password.size() + 1);
        if (cstr)
        {
            std::strcpy(cstr, password.c_str());
        }
        return cstr;
    }
    return nullptr;
}

void nexilis_server_data_set_inet_udp(nexilis_ServerData* server_data, const char* server_address)
{
    if (server_data && server_data->data && server_address)
    {
        server_data->data->setInetUDP(server_address);
    }
}

const char* nexilis_server_data_get_inet_udp_server_address(const nexilis_ServerData* server_data)
{
    if (server_data && server_data->data)
    {
        std::string address = server_data->data->getInetUDPServerAddress();
        char* cstr = (char*)malloc(address.size() + 1);
        if (cstr)
        {
            std::strcpy(cstr, address.c_str());
        }
        return cstr;
    }
    return nullptr;
}

void nexilis_server_data_set_inet_tcp(nexilis_ServerData* server_data, const char* server_address)
{
    if (server_data && server_data->data && server_address)
    {
        server_data->data->setInetTCP(server_address);
    }
}

const char* nexilis_server_data_get_inet_tcp_server_address(const nexilis_ServerData* server_data)
{
    if (server_data && server_data->data)
    {
        std::string address = server_data->data->getInetTCPServerAddress();
        char* cstr = (char*)malloc(address.size() + 1);
        if (cstr)
        {
            std::strcpy(cstr, address.c_str());
        }
        return cstr;
    }
    return nullptr;
}

void nexilis_server_data_set_boost_tcp(nexilis_ServerData* server_data, const char* server_address)
{
    if (server_data && server_data->data && server_address)
    {
        server_data->data->setBoostTCP(server_address);
    }
}

const char* nexilis_server_data_get_boost_tcp_server_address(const nexilis_ServerData* server_data)
{
    if (server_data && server_data->data)
    {
        std::string address = server_data->data->getBoostTCPServerAddress();
        char* cstr = (char*)malloc(address.size() + 1);
        if (cstr)
        {
            std::strcpy(cstr, address.c_str());
        }
        return cstr;
    }
    return nullptr;
}

void nexilis_server_data_set_boost_udp(nexilis_ServerData* server_data, const char* server_address)
{
    if (server_data && server_data->data && server_address)
    {
        server_data->data->setBoostUDP(server_address);
    }
}

const char* nexilis_server_data_get_boost_udp_server_address(const nexilis_ServerData* server_data)
{
    if (server_data && server_data->data)
    {
        std::string address = server_data->data->getBoostUDPServerAddress();
        char* cstr = (char*)malloc(address.size() + 1);
        if (cstr)
        {
            std::strcpy(cstr, address.c_str());
        }
        return cstr;
    }
    return nullptr;
}

void nexilis_server_data_set_unix_dgram_server_path(nexilis_ServerData* server_data, const char* socket_path)
{
    if (server_data && server_data->data && socket_path)
    {
        server_data->data->setUnixDgramServerPath(socket_path);
    }
}

const char* nexilis_server_data_get_unix_dgram_server_path(const nexilis_ServerData* server_data)
{
    if (server_data && server_data->data)
    {
        std::string path = server_data->data->getUnixDgramServerPath();
        char* cstr = (char*)malloc(path.size() + 1);
        if (cstr)
        {
            std::strcpy(cstr, path.c_str());
        }
        return cstr;
    }
    return nullptr;
}

void nexilis_server_data_set_unix_stream_server_path(nexilis_ServerData* server_data, const char* socket_path)
{
    if (server_data && server_data->data)
    {
        server_data->data->setUnixStreamServerPath(socket_path);
    }
}

const char* nexilis_server_data_get_unix_stream_server_path(const nexilis_ServerData* server_data)
{
    if (server_data && server_data->data)
    {
        std::string path = server_data->data->getUnixStreamServerPath();
        char* cstr = (char*)malloc(path.size() + 1);
        if (cstr)
        {
            std::strcpy(cstr, path.c_str());
        }
        return cstr;
    }
    return nullptr;
}

nexilis_ClientAPI* nexilis_client_api_create(nexilis_ServerData* server_data)
{
    if (server_data && server_data->data)
    {
        auto client_api = new nexilis_ClientAPI();
        auto cpp_client_api = new nexilis::client::ClientAPI(*server_data->data);
        client_api->api = cpp_client_api;
        return client_api;
    }
    return nullptr;
}

void nexilis_client_api_destroy(nexilis_ClientAPI* client_api)
{
    if (client_api)
    {
        if (client_api->api)
        {
            delete client_api->api;
            client_api->api = nullptr;
        }
        delete client_api;
    }
}

bool nexilis_client_api_is_inet_udp_ready(const nexilis_ClientAPI* client_api)
{
    if (client_api && client_api->api)
    {
        return client_api->api->IsInetUDPReady();
    }
    assert(!"Undefined Client API");
}

bool nexilis_client_api_is_inet_tcp_ready(const nexilis_ClientAPI* client_api)
{
    if (client_api && client_api->api)
    {
        return client_api->api->isInetTCPReady();
    }
    assert(!"Undefined Client API");
}

bool nexilis_client_api_is_boost_tcp_ready(const nexilis_ClientAPI* client_api)
{
    if (client_api && client_api->api)
    {
        return client_api->api->isBoostTCPReady();
    }
    assert(!"Undefined Client API");
}

bool nexilis_client_api_is_boost_udp_ready(const nexilis_ClientAPI* client_api)
{
    if (client_api && client_api->api)
    {
        return client_api->api->isBoostUDPReady();
    }
    assert(!"Undefined Client API");
}

bool nexilis_client_api_is_unix_dgram_ready(const nexilis_ClientAPI* client_api)
{
    if (client_api && client_api->api)
    {
        return client_api->api->isUnixDgramReady();
    }
    assert(!"Undefined Client API");
}

bool nexilis_client_api_is_unix_stream_ready(const nexilis_ClientAPI* client_api)
{
    if (client_api && client_api->api)
    {
        return client_api->api->isUnixStreamReady();
    }
    assert(!"Undefined Client API");
}

void nexilis_client_api_wait_until_inet_udp_ready(nexilis_ClientAPI* client_api)
{
    if (client_api && client_api->api)
    {
        client_api->api->waitUntilInetUDPReady();
    }
    assert(!"Undefined Client API");
}

void nexilis_client_api_wait_until_inet_tcp_ready(nexilis_ClientAPI* client_api)
{
    if (client_api && client_api->api)
    {
        client_api->api->waitUntilInetTCPReady();
    }
    assert(!"Undefined Client API");
}

void nexilis_client_api_wait_until_boost_tcp_ready(nexilis_ClientAPI* client_api)
{
    if (client_api && client_api->api)
    {
        client_api->api->waitUntilBoostTCPReady();
    }
    assert(!"Undefined Client API");
}

void nexilis_client_api_wait_until_boost_udp_ready(nexilis_ClientAPI* client_api)
{
    if (client_api && client_api->api)
    {
        client_api->api->waitUntilBoostUDPReady();
    }
    assert(!"Undefined Client API");
}

void nexilis_client_api_wait_until_unix_dgram_ready(nexilis_ClientAPI* client_api)
{
    if (client_api && client_api->api)
    {
        client_api->api->waitUntilUnixDgramReady();
    }
    assert(!"Undefined Client API");
}

void nexilis_client_api_wait_until_unix_stream_ready(nexilis_ClientAPI* client_api)
{
    if (client_api && client_api)
    {
        client_api->api->waitUntilUnixStreamReady();
    }
    assert(!"Undefined Client API");
}

uint64_t nexilis_client_api_get_client_id(const nexilis_ClientAPI* client_api)
{
    if (client_api && client_api->api)
    {
        return client_api->api->getClientId();
    }
    return 0;
}

const char* nexilis_client_api_get_client_username(const nexilis_ClientAPI* client_api)
{
    if (client_api && client_api->api)
    {
        std::string username = client_api->api->getClientUserName();
        char* cstr = (char*)malloc(username.size() + 1);
        if (cstr)
        {
            std::strcpy(cstr, username.c_str());
        }
        return cstr;
    }
    return nullptr;
}

const char* nexilis_client_api_get_client_password(const nexilis_ClientAPI* client_api)
{
    if (client_api && client_api->api)
    {
        std::string password = client_api->api->getClientPassword();
        char* cstr = (char*)malloc(password.size() + 1);
        if (cstr)
        {
            std::strcpy(cstr, password.c_str());
        }
        return cstr;
    }
    return nullptr;
}

const char* nexilis_client_api_get_inet_udp_server_address(const nexilis_ClientAPI* client_api)
{
    if (client_api && client_api->api)
    {
        std::string address = client_api->api->getInetUDPServerAddress();
        char* cstr = (char*)malloc(address.size() + 1);
        if (cstr)
        {
            std::strcpy(cstr, address.c_str());
        }
        return cstr;
    }
    return nullptr;
}

const char* nexilis_client_api_get_inet_tcp_server_address(const nexilis_ClientAPI* client_api)
{
    if (client_api && client_api->api)
    {
        std::string address = client_api->api->getInetTCPServerAddress();
        char* cstr = (char*)malloc(address.size() + 1);
        if (cstr)
        {
            std::strcpy(cstr, address.c_str());
        }
        return cstr;
    }
    return nullptr;
}

const char* nexilis_client_api_get_boost_tcp_server_address(const nexilis_ClientAPI* client_api)
{
    if (client_api && client_api->api)
    {
        std::string address = client_api->api->getBoostTCPServerAddress();
        char* cstr = (char*)malloc(address.size() + 1);
        if (cstr)
        {
            std::strcpy(cstr, address.c_str());
        }
        return cstr;
    }
    return nullptr;
}

const char* nexilis_client_api_get_boost_udp_server_address(const nexilis_ClientAPI* client_api)
{
    if (client_api && client_api->api)
    {
        std::string address = client_api->api->getBoostUDPServerAddress();
        char* cstr = (char*)malloc(address.size() + 1);
        if (cstr)
        {
            std::strcpy(cstr, address.c_str());
        }
        return cstr;
    }
    return nullptr;
}

const char* nexilis_client_api_get_unix_dgram_path(const nexilis_ClientAPI* client_api)
{
    if (client_api && client_api->api)
    {
        std::string path = client_api->api->getUnixDgramPath();
        char* cstr = (char*)malloc(path.size() + 1);
        if (cstr)
        {
            std::strcpy(cstr, path.c_str());
        }
        return cstr;
    }
    return nullptr;
}

const char* nexilis_client_api_get_unix_stream_path(const nexilis_ClientAPI* client_api)
{
    if (client_api && client_api->api)
    {
        std::string address = client_api->api->getUnixStreamPath();
        char* cstr = (char*)malloc(address.size() + 1);
        if (cstr)
        {
            std::strcpy(cstr, address.c_str());
        }
        return cstr;
    }
    return nullptr;
}

nexilis_Room* nexilis_room_create(nexilis_ClientSession** clients, size_t num_clients, uint64_t creator_id, const char* name, nexilis_RoomContext context, uint32_t max_size)
{
    std::vector<nexilis::client::ClientAPI::ClientSession> client_sessions;
    for (size_t i = 0; i < num_clients; ++i)
    {
        client_sessions.emplace_back(std::move(*clients[i]->session));
    }
    uint64_t room_id = nexilis::Util::getRandomUint64();
    auto room_data = new nexilis_RoomData();
    room_data->data = new nexilis::RoomData(creator_id, std::string(name), room_id, static_cast<nexilis::RoomData::Context>(context), max_size);

    auto room = new nexilis_Room();
    room->room = new nexilis::client::ClientAPI::Room(*room_data->data, std::move(client_sessions));
    return room;
}

void nexilis_room_destroy(nexilis_Room* room)
{
    if (room)
    {
        if (room->room)
        {
            delete room->room;
            room->room = nullptr;
        }
        delete room;
    }
}

void nexilis_room_add_client(nexilis_Room* room, nexilis_ClientSession* client)
{
    if (room && room->room && client && client->session)
    {
        room->room->addClient(std::move(*client->session));
    }
}

void nexilis_room_remove_client(nexilis_Room* room, uint64_t client_id)
{
    if (room && room->room)
    {
        room->room->removeClient(client_id);
    }
}

nexilis_ClientSession** nexilis_room_get_clients(nexilis_ClientAPI* client_api, const nexilis_Room* room, size_t* num_clients)
{
    if (client_api && client_api->api && room && room->room && num_clients)
    {
        const auto& clients = room->room->getClients();
        *num_clients = clients.size();
        nexilis_ClientSession** client_array = (nexilis_ClientSession**)malloc(sizeof(nexilis_ClientSession*) * (*num_clients));
        for (size_t i = 0; i < *num_clients; ++i)
        {
            client_array[i]->session = new nexilis::client::ClientAPI::ClientSession(clients[i].getId(), client_api->api);
        }
        return client_array;
    }
    return nullptr;
}

void nexilis_room_add_message(nexilis_Room* room, nexilis_Communication* communication)
{
    if (room && room->room && communication && communication->communication)
    {
        room->room->addMessage(std::move(*communication->communication));
    }
}

nexilis_Communication** nexilis_room_get_messages(const nexilis_Room* room, size_t* num_messages)
{
    if (room && room->room && num_messages)
    {
        const auto& messages = room->room->getMessages();
        *num_messages = messages.size();
        nexilis_Communication** message_array = (nexilis_Communication**)malloc(sizeof(nexilis_Communication*) * (*num_messages));
        for (size_t i = 0; i < *num_messages; ++i)
        {
            message_array[i]->communication = new nexilis::client::ClientAPI::Room::Communication(messages[i]);
        }
        return message_array;
    }
    return nullptr;
}

bool nexilis_room_contains_communication(const nexilis_Room* room, const nexilis_Communication* communication)
{
    if (room && room->room && communication && communication->communication)
    {
        return room->room->containsCommunication(*communication->communication);
    }
    assert(!"Undefined room");
}

bool nexilis_room_contains_communication_by_id(const nexilis_Room* room, uint64_t communication_id)
{
    if (room && room->room)
    {
        return room->room->containsCommunication(communication_id);
    }
    assert(!"Undefined room");
}

nexilis_Communication* nexilis_communication_create(const char* payload, nexilis_ClientSession* sender)
{
    if (sender && sender->session && payload)
    {
        auto communication = new nexilis_Communication();
        auto cpp_communication = new nexilis::client::ClientAPI::Room::Communication(payload, sender->session);
        communication->communication = cpp_communication;
        return communication;
    }
    return nullptr;
}

void nexilis_communication_destroy(nexilis_Communication* communication)
{
    if (communication)
    {
        if (communication->communication)
        {
            delete communication->communication;
            communication->communication = nullptr;
        }
        delete communication;
    }
}

const char* nexilis_communication_get_payload(const nexilis_Communication* communication)
{
    if (communication && communication->communication)
    {
        std::string payload = communication->communication->getPayload();
        char* cstr = (char*)malloc(payload.size() + 1);
        if (cstr)
        {
            std::strcpy(cstr, payload.c_str());
        }
        return cstr;
    }
    return nullptr;
}

const nexilis_ClientSession* nexilis_communication_get_client(const nexilis_Communication* communication)
{
    if (communication && communication->communication)
    {
        auto client_session = new nexilis_ClientSession();
        auto cpp_client_session = communication->communication->getClient();
        client_session->session = const_cast<nexilis::client::ClientAPI::ClientSession*>(cpp_client_session);
        return client_session;
    }
    return nullptr;
}

uint64_t nexilis_communication_get_id(const nexilis_Communication* communication)
{
    if (communication && communication->communication)
    {
        return communication->communication->getId();
    }
    return 0;
}
