#ifndef NEXILISC_PROTOCOL_MANAGER_C_H
#define NEXILISC_PROTOCOL_MANAGER_C_H

#include <nexilisc/protocol_type_c.h>
#include <nexilisc/protocol_status_c.h>
#include <nexilisc/server/settings_c.h>

#ifdef __cplusplus
extern "C" {
#endif

// Protocol manager and it's data.
typedef struct nexilis_ProtocolManagerC nexilis_ProtocolManagerC;
typedef struct nexilis_ProtocolDataC nexilis_ProtocolDataC;

// Different protocols.
typedef struct nexilis_UnixStreamServer nexilis_UnixStreamServer;

// ProtocolManager
nexilis_ProtocolManagerC* nexilis_protocol_manager_create();
void nexilis_protocol_manager_destroy(nexilis_ProtocolManagerC* manager);

// ProtocolData
nexilis_ProtocolDataC* nexilis_protocol_data_create(nexilis_ProtocolTypeC type);
void nexilis_protocol_data_destroy(nexilis_ProtocolDataC* data);
nexilis_ProtocolTypeC nexilis_protocol_data_get_type(const nexilis_ProtocolDataC* data);
nexilis_ProtocolStatusC nexilis_protocol_data_get_status(const nexilis_ProtocolDataC* data);
size_t nexilis_protocol_get_id(const nexilis_ProtocolDataC* data);

// Protocol creation functions.
nexilis_UnixStreamServer* nexilis_protocol_create_unix_stream_server(nexilis_ProtocolManagerC* manager, nexilis_server_SettingsC* settings, const char* socket_path);
void nexilis_protocol_unix_stream_server_destroy(nexilis_UnixStreamServer* server);

#ifdef __cplusplus
}
#endif

#endif
