/* Copyright (C) 2026 Valtteri Viirret
   This file is part of the Nexilis Project.

   This file is free software: you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as
   published by the Free Software Foundation, either version 3 of the
   License, or (at your option) any later version.

   This file is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with this file.  If not, see <https://gnu.org>. */

#include <nexilisc/client/client_api_c.h>
#include <nexilisc/client/client_session_c.h>
#include <nexilisc/room_data_c.h>

nexilis_ClientAPI* nexilis_client_api_create(nexilis_ClientConfigC* config)
{
    if (config && config->data)
    {
        auto client_api = new nexilis_ClientAPI();
        auto cpp_client_api = new nexilis::client::ClientAPI(*config->data);
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
    return false;
}

bool nexilis_client_api_is_inet_tcp_ready(const nexilis_ClientAPI* client_api)
{
    if (client_api && client_api->api)
    {
        return client_api->api->isInetTCPReady();
    }
    assert(!"Undefined Client API");
    return false;
}

bool nexilis_client_api_is_boost_tcp_ready(const nexilis_ClientAPI* client_api)
{
    if (client_api && client_api->api)
    {
        return client_api->api->isBoostTCPReady();
    }
    assert(!"Undefined Client API");
    return false;
}

bool nexilis_client_api_is_boost_udp_ready(const nexilis_ClientAPI* client_api)
{
    if (client_api && client_api->api)
    {
        return client_api->api->isBoostUDPReady();
    }
    assert(!"Undefined Client API");
    return false;
}

bool nexilis_client_api_is_unix_dgram_ready(const nexilis_ClientAPI* client_api)
{
    if (client_api && client_api->api)
    {
        return client_api->api->isUnixDgramReady();
    }
    assert(!"Undefined Client API");
    return false;
}

bool nexilis_client_api_is_unix_stream_ready(const nexilis_ClientAPI* client_api)
{
    if (client_api && client_api->api)
    {
        return client_api->api->isUnixStreamReady();
    }
    assert(!"Undefined Client API");
    return false;
}

bool nexilis_client_api_is_initialized(const nexilis_ClientAPI* client_api)
{
    if (client_api && client_api->api)
    {
        return client_api->api->isInitialized();
    }
    return false;
}

void nexilis_client_api_wait_until_inet_udp_ready(nexilis_ClientAPI* client_api)
{
    if (client_api && client_api->api)
    {
        client_api->api->waitUntilInetUDPReady();
        return;
    }
    assert(!"Undefined Client API");
}

void nexilis_client_api_wait_until_inet_tcp_ready(nexilis_ClientAPI* client_api)
{
    if (client_api && client_api->api)
    {
        client_api->api->waitUntilInetTCPReady();
        return;
    }
    assert(!"Undefined Client API");
}

void nexilis_client_api_wait_until_boost_tcp_ready(nexilis_ClientAPI* client_api)
{
    if (client_api && client_api->api)
    {
        client_api->api->waitUntilBoostTCPReady();
        return;
    }
    assert(!"Undefined Client API");
}

void nexilis_client_api_wait_until_boost_udp_ready(nexilis_ClientAPI* client_api)
{
    if (client_api && client_api->api)
    {
        client_api->api->waitUntilBoostUDPReady();
        return;
    }
    assert(!"Undefined Client API");
}

void nexilis_client_api_wait_until_unix_dgram_ready(nexilis_ClientAPI* client_api)
{
    if (client_api && client_api->api)
    {
        client_api->api->waitUntilUnixDgramReady();
        return;
    }
    assert(!"Undefined Client API");
}

void nexilis_client_api_wait_until_unix_stream_ready(nexilis_ClientAPI* client_api)
{
    if (client_api && client_api->api)
    {
        client_api->api->waitUntilUnixStreamReady();
        return;
    }
    assert(!"Undefined Client API");
}

uint64_t nexilis_client_api_get_client_id(const nexilis_ClientAPI* client_api)
{
    if (!client_api || !client_api->api)
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
    if (!client_api || !client_api->api)
    {
        return nullptr;
    }
    return getCString(client_api->api->getClientPassword());
}

const char* nexilis_client_api_get_inet_udp_server_address(const nexilis_ClientAPI* client_api)
{
    if (!client_api || !client_api->api)
    {
        return nullptr;
    }
    return getCString(client_api->api->getInetUDPServerAddress());
}

const char* nexilis_client_api_get_inet_tcp_server_address(const nexilis_ClientAPI* client_api)
{
    if (!client_api || !client_api->api)
    {
        return nullptr;
    }
    return getCString(client_api->api->getInetTCPServerAddress());
}

const char* nexilis_client_api_get_boost_tcp_server_address(const nexilis_ClientAPI* client_api)
{
    if (!client_api || !client_api->api)
    {
        return nullptr;
    }
    return getCString(client_api->api->getBoostTCPServerAddress());
}

const char* nexilis_client_api_get_boost_udp_server_address(const nexilis_ClientAPI* client_api)
{
    if (!client_api || !client_api->api)
    {
        return nullptr;
    }
    return getCString(client_api->api->getBoostUDPServerAddress());
}

const char* nexilis_client_api_get_unix_dgram_path(const nexilis_ClientAPI* client_api)
{
    if (!client_api || !client_api->api)
    {
        return nullptr;
    }
    return getCString(client_api->api->getUnixDgramPath());
}

const char* nexilis_client_api_get_unix_stream_path(const nexilis_ClientAPI* client_api)
{
    if (!client_api || !client_api->api)
    {
        return nullptr;
    }
    return getCString(client_api->api->getUnixStreamPath());
}

nexilis_RoomsCollection* nexilis_client_api_get_active_rooms(const nexilis_ClientAPI* client_api)
{
    if (!client_api || !client_api->api)
    {
        return nullptr;
    }
    auto* collection = new nexilis_RoomsCollection;
    collection->rooms = new std::vector<nexilis::client::Room>(client_api->api->getActiveRooms());
    return collection;
}

size_t nexilis_client_api_rooms_count(const nexilis_ClientAPI* client_api)
{
    if (!client_api || !client_api->api)
    {
        return 0;
    }
    return client_api->api->getActiveRooms().size();
}

size_t nexilis_client_api_client_room_id(const nexilis_ClientAPI* client_api)
{
    if (!client_api || !client_api->api)
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
    const_room->owned = false;
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
