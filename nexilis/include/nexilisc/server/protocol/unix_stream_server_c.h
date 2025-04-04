#ifdef __linux__

#ifndef NEXILISC_SERVER_PROTOCOL_AF_UNIX_STREAM_SERVER_C_H
#define NEXILISC_SERVER_PROTOCOL_AF_UNIX_STREAM_SERVER_C_H

#include <nexilisc/server/settings_c.h>
#include <nexilisc/protocol_manager_c.h>
#include <nexilisc/protocol_type_c.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct nexilis_UnixStreamServer nexilis_UnixStreamServer;

nexilis_UnixStreamServer* nexilis_create_unix_stream_server(nexilis_ProtocolManagerC* manager, nexilis_server_SettingsC* settings, const char* socket_path);
void nexilis_unix_stream_server_destroy(nexilis_UnixStreamServer* server);
nexilis_server_SettingsC nexilis_unix_stream_server_get_settings(nexilis_UnixStreamServer* server);
nexilis_ProtocolTypeC nexilis_unix_stream_server_get_type(nexilis_UnixStreamServer* server);

#ifdef __cplusplus
}
#endif

#endif

#endif
