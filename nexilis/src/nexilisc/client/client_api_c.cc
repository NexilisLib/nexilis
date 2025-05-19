#include <nexilisc/client/client_api_c.h>
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

const nexilis_RoomsCollection* nexilis_client_api_get_active_rooms(const nexilis_ClientAPI* client_api)
{
    static nexilis_RoomsCollection collection;
    collection.rooms = &client_api->api->getActiveRooms();
    return &collection;
}

