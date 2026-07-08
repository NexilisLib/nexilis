#include <nexilisc/server/server_config_c.h>

nexilis_server_ConfigC* nexilis_server_config_create()
{
    auto config = new nexilis_server_ConfigC();
    config->settings = new nexilis::server::ServerConfig();
    return config;
}

void nexilis_server_config_destroy(nexilis_server_ConfigC* config)
{
    if (config)
    {
        if (config->settings)
        {
            delete config->settings;
            config->settings = nullptr;
        }
        delete config;
    }
}

void nexilis_server_config_set_passphrase(nexilis_server_ConfigC* config, const char* password)
{
    if (config)
    {
        config->settings->setPassphrase(password);
    }
}

bool nexilis_server_config_is_passphrase(nexilis_server_ConfigC* config, const char* password)
{
    return config ? config->settings->isPassphrase(password) : false;
}

bool nexilis_server_config_has_passphrase(nexilis_server_ConfigC* config)
{
    return config ? config->settings->hasPassphrase() : false;
}

const char* nexilis_server_config_get_passphrase(nexilis_server_ConfigC* config)
{
    if (config)
    {
        static std::string passphrase = config->settings->getPassphrase();
        return passphrase.c_str();
    }
    return nullptr;
}

void nexilis_server_config_set_root_password(nexilis_server_ConfigC* config, const char* password)
{
    if (config)
    {
        config->settings->setRootPassword(password);
    }
}

bool nexilis_server_config_is_root_password(nexilis_server_ConfigC* config, const char* password)
{
    return config ? config->settings->isRootPassword(password) : false;
}

bool nexilis_server_config_has_root_password(nexilis_server_ConfigC* config)
{
    return config ? config->settings->hasRootPassword() : false;
}

const char* nexilis_server_config_get_root_password(nexilis_server_ConfigC* config)
{
    if (config)
    {
        static std::string rootPassword = config->settings->getRootPassword();
        return rootPassword.c_str();
    }
    return nullptr;
}

void nexilis_server_config_set_mode(nexilis_server_ConfigC* config, nexilis_server_AuthenticationModeC mode)
{
    if (config && config->settings)
    {
        switch (mode)
        {
            case AUTHENTICATION_MODE_EMPTY:
                config->settings->setMode(nexilis::server::AuthenticationMode::empty);
                break;
            case AUTHENTICATION_MODE_SKIP:
                config->settings->setMode(nexilis::server::AuthenticationMode::skip);
                break;
            case AUTHENTICATION_MODE_PASSWORD_PROTECTED:
                config->settings->setMode(nexilis::server::AuthenticationMode::password_protected);
                break;
            case AUTHENTICATION_MODE_ADMIN_ACCESS:
                config->settings->setMode(nexilis::server::AuthenticationMode::admin_access);
                break;
            case AUTHENTICATION_MODE_ROOT_ACCESS:
                config->settings->setMode(nexilis::server::AuthenticationMode::root_access);
                break;
        }
    }
}

nexilis_server_AuthenticationModeC nexilis_server_config_get_mode(nexilis_server_ConfigC* config)
{
    return config ? static_cast<nexilis_server_AuthenticationModeC>(config->settings->getMode()) : AUTHENTICATION_MODE_EMPTY;
}

void nexilis_server_config_set_tickrate(nexilis_server_ConfigC* config, float tickrate)
{
    if (config)
    {
        config->settings->setTickrate(tickrate);
    }
}

float nexilis_server_config_get_tickrate(nexilis_server_ConfigC* config)
{
    return config ? config->settings->getTickrate() : 0.f;
}
