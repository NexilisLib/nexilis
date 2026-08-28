#ifndef NEXILISC_CLIENT_CONFIG_C_H
#define NEXILISC_CLIENT_CONFIG_C_H

#include <nexilis/client/client_api.hh>

#ifdef __cplusplus
extern "C" {
#endif

struct nexilis_ClientConfigC
{
    nexilis::client::ClientConfig* data;
};

nexilis_ClientConfigC* nexilis_client_config_create();
void nexilis_client_config_destroy(nexilis_ClientConfigC* config);

void nexilis_client_config_set_password(nexilis_ClientConfigC* config, const char* password);
const char* nexilis_client_config_get_password(const nexilis_ClientConfigC* config);

void nexilis_client_config_set_mode(nexilis_ClientConfigC* config, int mode);

void nexilis_client_config_set_message_encryption(nexilis_ClientConfigC* config, int enabled);
int nexilis_client_config_get_message_encryption(const nexilis_ClientConfigC* config);

void nexilis_client_config_set_tls(nexilis_ClientConfigC* config, int enabled);
int nexilis_client_config_get_tls(const nexilis_ClientConfigC* config);

void nexilis_client_config_set_inet_udp(nexilis_ClientConfigC* config, const char* server_address);
const char* nexilis_client_config_get_inet_udp_server_address(const nexilis_ClientConfigC* config);

void nexilis_client_config_set_inet_tcp(nexilis_ClientConfigC* config, const char* server_address);
const char* nexilis_client_config_get_inet_tcp_server_address(const nexilis_ClientConfigC* config);

void nexilis_client_config_set_boost_tcp_address(nexilis_ClientConfigC* config, const char* server_address);
const char* nexilis_client_config_get_boost_tcp_server_address(const nexilis_ClientConfigC* config);

void nexilis_client_config_set_boost_udp_address(nexilis_ClientConfigC* config, const char* server_address);
const char* nexilis_client_config_get_boost_udp_server_address(const nexilis_ClientConfigC* config);

void nexilis_client_config_set_unix_dgram_server_path(nexilis_ClientConfigC* config, const char* socket_path);
const char* nexilis_client_config_get_unix_dgram_server_path(const nexilis_ClientConfigC* config);

void nexilis_client_config_set_unix_stream_server_path(nexilis_ClientConfigC* config, const char* socket_path);
const char* nexilis_client_config_get_unix_stream_server_path(const nexilis_ClientConfigC* config);

#ifdef __cplusplus
}
#endif


#endif
