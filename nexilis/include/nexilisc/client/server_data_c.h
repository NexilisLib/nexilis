#ifndef NEXILISC_SERVER_DATA_C_H
#define NEXILISC_SERVER_DATA_C_H

#include <nexilis/client/client_api.hh>

#ifdef __cplusplus
extern "C" {
#endif

struct nexilis_ServerData
{
    nexilis::client::ServerData* data;
};

nexilis_ServerData* nexilis_server_data_create();
void nexilis_server_data_destroy(nexilis_ServerData* server_data);

void nexilis_server_data_set_username(nexilis_ServerData* server_data, const char* username);
const char* nexilis_server_data_get_username(const nexilis_ServerData* server_data);
void nexilis_server_data_set_password(nexilis_ServerData* server_data, const char* password);
const char* nexilis_server_data_get_password(const nexilis_ServerData* server_data);

void nexilis_server_data_set_inet_udp(nexilis_ServerData* server_data, const char* server_address);
const char* nexilis_server_data_get_inet_udp_server_address(const nexilis_ServerData* server_data);

void nexilis_server_data_set_inet_tcp(nexilis_ServerData* server_data, const char* server_address);
const char* nexilis_server_data_get_inet_tcp_server_address(const nexilis_ServerData* server_data);

void nexilis_server_data_set_boost_tcp_address(nexilis_ServerData* server_data, const char* server_address);
const char* nexilis_server_data_get_boost_tcp_server_address(const nexilis_ServerData* server_data);

void nexilis_server_data_set_boost_udp_address(nexilis_ServerData* server_data, const char* server_address);
const char* nexilis_server_data_get_boost_udp_server_address(const nexilis_ServerData* server_data);

void nexilis_server_data_set_unix_dgram_server_path(nexilis_ServerData* server_data, const char* socket_path);
const char* nexilis_server_data_get_unix_dgram_server_path(const nexilis_ServerData* server_data);

void nexilis_server_data_set_unix_stream_server_path(nexilis_ServerData* server_data, const char* socket_path);
const char* nexilis_server_data_get_unix_stream_server_path(const nexilis_ServerData* server_data);

#ifdef __cplusplus
}
#endif


#endif
