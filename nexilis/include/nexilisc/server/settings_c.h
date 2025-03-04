#ifndef NEXILISC_SERVER_SETTINGS_C_H
#define NEXILISC_SERVER_SETTINGS_C_H

#include <nexilisc/server/authentication_mode_c.h>

#include <nexilis/server/settings.hh>

#ifdef __cplusplus
extern "C" {
#endif

struct nexilis_server_SettingsC
{
    nexilis::server::Settings* settings;
};

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

#ifdef __cplusplus
}
#endif

#endif
