#include <nexilisc/client/client_api_c.h>
#include <nexilisc/client/client_session_c.h>
#include <nexilisc/room_data_c.h>

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
    if (!client_api && !client_api->api)
    {
        return 0;
    }
    return client_api->api->getClientId();
}

// Helper to create new c-string.
const char* getCString(const std::string& str)
{
    char* cstr = (char*)malloc(str.size() + 1);
    if (cstr)
    {
        std::strcpy(cstr, str.c_str());
    }
    return cstr;
}

const char* nexilis_client_api_get_client_username(const nexilis_ClientAPI* client_api, uint64_t client_id)
{
    if (!client_api || !client_api->api)
    {
        return nullptr;
    }
    return getCString(client_api->api->getClientUsername(client_id));
}

const char* nexilis_client_api_get_client_password(const nexilis_ClientAPI* client_api)
{
    if (!client_api && !client_api->api)
    {
        return nullptr;
    }
    return getCString(client_api->api->getClientPassword());
}

const char* nexilis_client_api_get_inet_udp_server_address(const nexilis_ClientAPI* client_api)
{
    if (!client_api && !client_api->api)
    {
        return nullptr;
    }
    return getCString(client_api->api->getInetUDPServerAddress());
}

const char* nexilis_client_api_get_inet_tcp_server_address(const nexilis_ClientAPI* client_api)
{
    if (!client_api && !client_api->api)
    {
        return nullptr;
    }
    return getCString(client_api->api->getInetTCPServerAddress());
}

const char* nexilis_client_api_get_boost_tcp_server_address(const nexilis_ClientAPI* client_api)
{
    if (!client_api && !client_api->api)
    {
        return nullptr;
    }
    return getCString(client_api->api->getBoostTCPServerAddress());
}

const char* nexilis_client_api_get_boost_udp_server_address(const nexilis_ClientAPI* client_api)
{
    if (!client_api && !client_api->api)
    {
        return nullptr;
    }
    return getCString(client_api->api->getBoostUDPServerAddress());
}

const char* nexilis_client_api_get_unix_dgram_path(const nexilis_ClientAPI* client_api)
{
    if (!client_api && !client_api->api)
    {
        return nullptr;
    }
    return getCString(client_api->api->getUnixDgramPath());
}

const char* nexilis_client_api_get_unix_stream_path(const nexilis_ClientAPI* client_api)
{
    if (!client_api && !client_api->api)
    {
        return nullptr;
    }
    return getCString(client_api->api->getUnixStreamPath());
}

nexilis_RoomsCollection* nexilis_client_api_get_active_rooms(const nexilis_ClientAPI* client_api)
{
    auto* collection = new nexilis_RoomsCollection;
    collection->rooms = &client_api->api->getActiveRooms();
    return collection;
}

size_t nexilis_client_api_rooms_count(const nexilis_ClientAPI* client_api)
{
    if (!client_api && !client_api->api)
    {
        return 0;
    }
    return client_api->api->getActiveRooms().size();
}

size_t nexilis_client_api_client_room_id(const nexilis_ClientAPI* client_api)
{
    if (!client_api && !client_api->api)
    {
        return 0;
    }
    return client_api->api->clientRoomId();
}

nexilis_Room* nexilis_client_api_get_room(const nexilis_ClientAPI* client_api, uint64_t room_id)
{
    if (!client_api || !client_api->api)
    {
        return nullptr;
    }

    auto room = client_api->api->getRoom(room_id);
    auto const_room = new nexilis_Room;
    const_room->room = room;
    return const_room;
}

nexilis_ClientSession* nexilis_client_api_get_client_from_room(const nexilis_ClientAPI* client_api, uint64_t client_id)
{
    if (!client_api || !client_api->api)
    {
        return nullptr;
    }

    auto client = client_api->api->getClientFromRoom(client_id);
    auto client_session = new nexilis_ClientSession;
    client_session->client = client;
    return client_session;
}
