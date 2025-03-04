#ifndef NEXILISC_PROTOCOL_MANAGER_C_H
#define NEXILISC_PROTOCOL_MANAGER_C_H

#include <nexilisc/protocol_type_c.h>
#include <nexilisc/protocol_status_c.h>

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// Server protocol settings.
typedef struct nexilis_server_SettingsC nexilis_server_SettingsC;

// Protocol manager and it's data.
typedef struct nexilis_ProtocolManagerC nexilis_ProtocolManagerC;
typedef struct nexilis_ProtocolDataC nexilis_ProtocolDataC;

// Different protocols.
typedef struct nexilis_UnixStreamServer nexilis_UnixStreamServer;

// Enum for AuthenticationMode
typedef enum {
    AUTHENTICATION_MODE_FREE,
    AUTHENTICATION_MODE_PASSWORD_PROTECTED,
    AUTHENTICATION_MODE_WHITELISTED
} nexilis_server_AuthenticationModeC;

// Create and destroy Settings.
nexilis_server_SettingsC* nexilis_settings_create();
void nexilis_settings_destroy(nexilis_server_SettingsC* settings);

// Normal password
void nexilis_settings_set_passphrase(nexilis_server_SettingsC* settings, const char* password);
bool nexilis_settings_is_passphrase(nexilis_server_SettingsC* settings, const char* password);
bool nexilis_settings_has_passphrase(nexilis_server_SettingsC* settings);
const char* nexilis_settings_get_passphrase(nexilis_server_SettingsC* settings);

// Root password
void nexilis_settings_set_root_password(nexilis_server_SettingsC* settings, const char* password);
bool nexilis_settings_is_root_password(nexilis_server_SettingsC* settings, const char* password);
bool nexilis_settings_has_root_password(nexilis_server_SettingsC* settings);
const char* nexilis_settings_get_root_password(nexilis_server_SettingsC* settings);

// Authentication mode
void nexilis_settings_set_mode(nexilis_server_SettingsC* settings, nexilis_server_AuthenticationModeC mode);
nexilis_server_AuthenticationModeC nexilis_settings_get_mode(nexilis_server_SettingsC* settings);

// Tickrate
void nexilis_settings_set_tickrate(nexilis_server_SettingsC* settings, float tickrate);
float nexilis_settings_get_tickrate(nexilis_server_SettingsC* settings);

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
