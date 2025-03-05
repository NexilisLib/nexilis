#ifndef NEXILISC_CLIENT_API_C_H
#define NEXILISC_CLIENT_API_C_H

#include <nexilis/client/client_api.hh>

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

struct nexilis_ClientAPI
{
    nexilis::client::ClientAPI* api;
};

typedef struct nexilis_ServerData nexilis_ServerData;
typedef struct nexilis_ClientSession nexilis_ClientSession;
typedef struct nexilis_Room nexilis_Room;
typedef struct nexilis_Communication nexilis_Communication;

// ServerData API
nexilis_ServerData* nexilis_server_data_create();
void nexilis_server_data_destroy(nexilis_ServerData* server_data);
void nexilis_server_data_set_username(nexilis_ServerData* server_data, const char* username);
const char* nexilis_server_data_get_username(const nexilis_ServerData* server_data);
void nexilis_server_data_set_password(nexilis_ServerData* server_data, const char* password);
const char* nexilis_server_data_get_password(const nexilis_ServerData* server_data);
void nexilis_server_data_set_inet_udp(nexilis_ServerData* server_data, const char* server_address, uint16_t port);
const char* nexilis_server_data_get_inet_udp_server_address(const nexilis_ServerData* server_data);
uint16_t nexilis_server_data_get_inet_udp_server_port(const nexilis_ServerData* server_data);
void nexilis_server_data_set_inet_tcp(nexilis_ServerData* server_data, const char* server_address, uint16_t port);
const char* nexilis_server_data_get_inet_tcp_server_address(const nexilis_ServerData* server_data);
uint16_t nexilis_server_data_get_inet_tcp_server_port(const nexilis_ServerData* server_data);
void nexilis_server_data_set_boost_tcp(nexilis_ServerData* server_data, const char* server_address, uint16_t port);
const char* nexilis_server_data_get_boost_tcp_server_address(const nexilis_ServerData* server_data);
uint16_t nexilis_server_data_get_boost_tcp_server_port(const nexilis_ServerData* server_data);
void nexilis_server_data_set_boost_udp(nexilis_ServerData* server_data, const char* server_address, uint16_t port);
const char* nexilis_server_data_get_boost_udp_server_address(const nexilis_ServerData* server_data);
uint16_t nexilis_server_data_get_boost_udp_server_port(const nexilis_ServerData* server_data);
void nexilis_server_data_set_unix_dgram_server_path(nexilis_ServerData* server_data, const char* socket_path);
const char* nexilis_server_data_get_unix_dgram_server_path(const nexilis_ServerData* server_data);
void nexilis_server_data_set_unix_stream_server_path(nexilis_ServerData* server_data, const char* socket_path);
const char* nexilis_server_data_get_unix_stream_server_path(const nexilis_ServerData* server_data);

// ClientAPI API
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
uint16_t nexilis_client_api_get_inet_udp_port_number(const nexilis_ClientAPI* client_api);
const char* nexilis_client_api_get_inet_tcp_server_address(const nexilis_ClientAPI* client_api);
uint16_t nexilis_client_api_get_inet_tcp_port_number(const nexilis_ClientAPI* client_api);
const char* nexilis_client_api_get_boost_tcp_server_address(const nexilis_ClientAPI* client_api);
uint16_t nexilis_client_api_get_boost_tcp_server_port_number(const nexilis_ClientAPI* client_api);
const char* nexilis_client_api_get_boost_udp_server_address(const nexilis_ClientAPI* client_api);
uint16_t nexilis_client_api_get_boost_udp_server_port_number(const nexilis_ClientAPI* client_api);
const char* nexilis_client_api_get_unix_dgram_path(const nexilis_ClientAPI* client_api);
const char* nexilis_client_api_get_unix_stream_path(const nexilis_ClientAPI* client_api);

// Room API
// TODO
//nexilis_Room* nexilis_room_create(const char* room_data, nexilis_ClientSession** clients, size_t num_clients);
void nexilis_room_destroy(nexilis_Room* room);
void nexilis_room_add_client(nexilis_Room* room, nexilis_ClientSession* client);
void nexilis_room_remove_client(nexilis_Room* room, uint64_t client_id);
nexilis_ClientSession** nexilis_room_get_clients(const nexilis_Room* room, size_t* num_clients);
void nexilis_room_add_message(nexilis_Room* room, nexilis_Communication* communication);
nexilis_Communication** nexilis_room_get_messages(const nexilis_Room* room, size_t* num_messages);
bool nexilis_room_contains_communication(const nexilis_Room* room, const nexilis_Communication* communication);
bool nexilis_room_contains_communication_by_id(const nexilis_Room* room, uint64_t communication_id);

// Communication API
nexilis_Communication* nexilis_communication_create(const char* payload, nexilis_ClientSession* sender);
void nexilis_communication_destroy(nexilis_Communication* communication);
const char* nexilis_communication_get_payload(const nexilis_Communication* communication);
const nexilis_ClientSession* nexilis_communication_get_client(const nexilis_Communication* communication);
uint64_t nexilis_communication_get_id(const nexilis_Communication* communication);

#ifdef __cplusplus
}
#endif

#endif
