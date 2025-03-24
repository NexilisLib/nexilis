#ifndef NEXILISC_SERVER_BOOST_TCP_SERVER_C_H
#define NEXILISC_SERVER_BOOST_TCP_SERVER_C_H

#include <nexilisc/protocol_manager_c.h>
#include <nexilisc/server/settings_c.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct nexilis_BoostTCPServer nexilis_BoostTCPServer;

nexilis_BoostTCPServer* nexilis_create_boost_tcp_server(nexilis_ProtocolManagerC* manager, nexilis_server_SettingsC* settings);
void nexilis_boost_tcp_server_destroy(nexilis_BoostTCPServer* server);
nexilis_server_SettingsC nexilis_boost_tcp_server_get_settings(nexilis_BoostTCPServer* server);
nexilis_ProtocolTypeC nexilis_boost_tcp_server_get_type(nexilis_BoostTCPServer* server);

#ifdef __cplusplus
}
#endif

#endif
