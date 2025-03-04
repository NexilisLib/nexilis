#include <nexilisc/server/settings_c.h>

nexilis_server_SettingsC* nexilis_settings_create()
{
    auto settings = new nexilis_server_SettingsC();
    settings->settings = new nexilis::server::Settings();
    return settings;
}

void nexilis_settings_destroy(nexilis_server_SettingsC* settings)
{
    if (settings)
    {
        if (settings->settings)
        {
            delete settings->settings;
            settings->settings = nullptr;
        }
        delete settings;
    }
}

void nexilis_settings_set_passphrase(nexilis_server_SettingsC* settings, const char* password)
{
    if (settings)
    {
        settings->settings->setPassphrase(password);
    }
}

bool nexilis_settings_is_passphrase(nexilis_server_SettingsC* settings, const char* password)
{
    return settings ? settings->settings->isPassphrase(password) : false;
}

bool nexilis_settings_has_passphrase(nexilis_server_SettingsC* settings)
{
    return settings ? settings->settings->hasPassphrase() : false;
}

const char* nexilis_settings_get_passphrase(nexilis_server_SettingsC* settings)
{
    if (settings)
    {
        static std::string passphrase = settings->settings->getPassphrase();
        return passphrase.c_str();
    }
    return nullptr;
}

void nexilis_settings_set_root_password(nexilis_server_SettingsC* settings, const char* password)
{
    if (settings)
    {
        settings->settings->setRootPassword(password);
    }
}

bool nexilis_settings_is_root_password(nexilis_server_SettingsC* settings, const char* password)
{
    return settings ? settings->settings->isRootPassword(password) : false;
}

bool nexilis_settings_has_root_password(nexilis_server_SettingsC* settings)
{
    return settings ? settings->settings->hasRootPassword() : false;
}

const char* nexilis_settings_get_root_password(nexilis_server_SettingsC* settings)
{
    if (settings)
    {
        static std::string rootPassword = settings->settings->getRootPassword();
        return rootPassword.c_str();
    }
    return nullptr;
}

void nexilis_settings_set_mode(nexilis_server_SettingsC* settings, nexilis_server_AuthenticationModeC mode)
{
    if (settings && settings->settings)
    {
        switch (mode)
        {
            case AUTHENTICATION_MODE_FREE:
                settings->settings->setMode(nexilis::server::Settings::AuthenticationMode::free);
                break;
            case AUTHENTICATION_MODE_PASSWORD_PROTECTED:
                settings->settings->setMode(nexilis::server::Settings::AuthenticationMode::passwordProtected);
                break;
            case AUTHENTICATION_MODE_WHITELISTED:
                settings->settings->setMode(nexilis::server::Settings::AuthenticationMode::whiteListed);
                break;
        }
    }
}

nexilis_server_AuthenticationModeC nexilis_settings_get_mode(nexilis_server_SettingsC* settings)
{
    return settings ? static_cast<nexilis_server_AuthenticationModeC>(settings->settings->getMode()) : AUTHENTICATION_MODE_FREE;
}

void nexilis_settings_set_tickrate(nexilis_server_SettingsC* settings, float tickrate)
{
    if (settings)
    {
        settings->settings->setTickrate(tickrate);
    }
}

float nexilis_settings_get_tickrate(nexilis_server_SettingsC* settings)
{
    return settings ? settings->settings->getTickrate() : 0.f;
}
