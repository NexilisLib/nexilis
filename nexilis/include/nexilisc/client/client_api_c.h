#ifndef NEXILISC_CLIENT_API_C_H
#define NEXILISC_CLIENT_API_C_H

#include <nexilisc/client/server_data_c.h>
#include <nexilisc/client/rooms_collection.h>

#include <nexilis/client/client_api.hh>

#ifdef __cplusplus
extern "C" {
#endif

struct nexilis_ClientAPI
{
    nexilis::client::ClientAPI* api;
};

nexilis_ClientAPI* nexilis_client_api_create(nexilis_ServerData* server_data);
void nexilis_client_api_destroy(nexilis_ClientAPI* client_api);
bool nexilis_client_api_is_inet_udp_ready(const nexilis_ClientAPI* client_api);
bool nexilis_client_api_is_inet_tcp_ready(const nexilis_ClientAPI* client_api);
bool nexilis_client_api_is_boost_tcp_ready(const nexilis_ClientAPI* client_api);
bool nexilis_client_api_is_boost_udp_ready(const nexilis_ClientAPI* client_api);
bool nexilis_client_api_is_unix_dgram_ready(const nexilis_ClientAPI* client_api);
bool nexilis_client_api_is_unix_stream_ready(const nexilis_ClientAPI* client_api);
void nexilis_client_api_wait_until_inet_udp_ready(nexilis_ClientAPI* client_api);
void nexilis_client_api_wait_until_inet_tcp_ready(nexilis_ClientAPI* client_api);
void nexilis_client_api_wait_until_boost_tcp_ready(nexilis_ClientAPI* client_api);
void nexilis_client_api_wait_until_boost_udp_ready(nexilis_ClientAPI* client_api);
void nexilis_client_api_wait_until_unix_dgram_ready(nexilis_ClientAPI* client_api);
void nexilis_client_api_wait_until_unix_stream_ready(nexilis_ClientAPI* client_api);
uint64_t nexilis_client_api_get_client_id(const nexilis_ClientAPI* client_api);
const char* nexilis_client_api_get_client_username(const nexilis_ClientAPI* client_api);
const char* nexilis_client_api_get_client_password(const nexilis_ClientAPI* client_api);
const char* nexilis_client_api_get_inet_udp_server_address(const nexilis_ClientAPI* client_api);
const char* nexilis_client_api_get_inet_tcp_server_address(const nexilis_ClientAPI* client_api);
const char* nexilis_client_api_get_boost_tcp_server_address(const nexilis_ClientAPI* client_api);
const char* nexilis_client_api_get_boost_udp_server_address(const nexilis_ClientAPI* client_api);
const char* nexilis_client_api_get_unix_dgram_path(const nexilis_ClientAPI* client_api);
const char* nexilis_client_api_get_unix_stream_path(const nexilis_ClientAPI* client_api);

nexilis_RoomsCollection nexilis_client_api_get_active_rooms(const nexilis_ClientAPI* client_api);

#ifdef __cplusplus
}
#endif

#endif
