#ifndef NEXILISC_CLIENT_PROTOCOL_BOOST_TCP_CLIENT_C_H
#define NEXILISC_CLIENT_PROTOCOL_BOOST_TCP_CLIENT_C_H

#include <nexilisc/protocol_manager_c.h>
#include <nexilisc/client/client_api_c.h>

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct nexilis_BoostTCPClient nexilis_BoostTCPClient;

nexilis_BoostTCPClient* nexilis_boost_tcp_client_create(nexilis_ProtocolManagerC* manager, nexilis_ClientAPI* client_api);
void nexilis_boost_tcp_client_destroy(nexilis_BoostTCPClient* client);
nexilis_ProtocolTypeC nexilis_boost_tcp_client_get_type(nexilis_BoostTCPClient* client);
void nexilis_boost_tcp_client_start(nexilis_BoostTCPClient* client);
void nexilis_boost_tcp_client_stop(nexilis_BoostTCPClient* client);
void nexilis_boost_tcp_client_send_message(nexilis_BoostTCPClient* client, const uint8_t message[], size_t message_size);
void nexilis_boost_tcp_client_send_message_with_callback(nexilis_BoostTCPClient* client, const uint8_t message[], size_t message_size, void (*callback)(const uint8_t*, size_t));
bool nexilis_boost_tcp_client_is_connected(nexilis_BoostTCPClient* client);

#ifdef __cplusplus
}
#endif

#endif
