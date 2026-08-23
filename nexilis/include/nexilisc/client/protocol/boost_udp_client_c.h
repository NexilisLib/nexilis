#ifndef NEXILISC_CLIENT_PROTOCOL_BOOST_UDP_CLIENT_C_H
#define NEXILISC_CLIENT_PROTOCOL_BOOST_UDP_CLIENT_C_H

#include <nexilisc/client/client_api_c.h>
#include <nexilisc/protocol_manager_c.h>

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct nexilis_BoostUDPClient nexilis_BoostUDPClient;

    nexilis_BoostUDPClient* nexilis_boost_udp_client_create(nexilis_ProtocolManagerC* manager, nexilis_ClientAPI* client_api);
    void nexilis_boost_udp_client_destroy(nexilis_BoostUDPClient* client);
    nexilis_ProtocolTypeC nexilis_boost_udp_client_get_type(nexilis_BoostUDPClient* client);
    void nexilis_boost_udp_client_start(nexilis_BoostUDPClient* client);
    void nexilis_boost_udp_client_stop(nexilis_BoostUDPClient* client);
    void nexilis_boost_udp_client_send_message(nexilis_BoostUDPClient* client, const uint8_t message[], size_t message_size);
    void nexilis_boost_udp_client_send_message_with_callback(nexilis_BoostUDPClient* client, const uint8_t message[], size_t message_size, void (*callback)(const uint8_t*, size_t));
    bool nexilis_boost_udp_client_is_connected(nexilis_BoostUDPClient* client);

#ifdef __cplusplus
}
#endif

#endif
