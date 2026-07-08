#ifndef NEXILISC_SERVER_CONFIG_C_H
#define NEXILISC_SERVER_CONFIG_C_H

#include <nexilisc/server/authentication_mode_c.h>

#include <nexilis/server/server_config.hh>

#ifdef __cplusplus
extern "C" {
#endif

struct nexilis_server_ConfigC
{
    nexilis::server::ServerConfig* settings;
};

// Create and destroy Config.
nexilis_server_ConfigC* nexilis_server_config_create();
void nexilis_server_config_destroy(nexilis_server_ConfigC* config);

// Normal password
void nexilis_server_config_set_passphrase(nexilis_server_ConfigC* config, const char* password);
bool nexilis_server_config_is_passphrase(nexilis_server_ConfigC* config, const char* password);
bool nexilis_server_config_has_passphrase(nexilis_server_ConfigC* config);
const char* nexilis_server_config_get_passphrase(nexilis_server_ConfigC* config);

// Root password
void nexilis_server_config_set_root_password(nexilis_server_ConfigC* config, const char* password);
bool nexilis_server_config_is_root_password(nexilis_server_ConfigC* config, const char* password);
bool nexilis_server_config_has_root_password(nexilis_server_ConfigC* config);
const char* nexilis_server_config_get_root_password(nexilis_server_ConfigC* config);

// Authentication mode
void nexilis_server_config_set_mode(nexilis_server_ConfigC* config, nexilis_server_AuthenticationModeC mode);
nexilis_server_AuthenticationModeC nexilis_server_config_get_mode(nexilis_server_ConfigC* config);

// Tickrate
void nexilis_server_config_set_tickrate(nexilis_server_ConfigC* config, float tickrate);
float nexilis_server_config_get_tickrate(nexilis_server_ConfigC* config);

#ifdef __cplusplus
}
#endif

#endif
