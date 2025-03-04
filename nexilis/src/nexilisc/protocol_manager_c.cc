#include <nexilis/protocol_manager.hh>
#include <nexilisc/protocol_manager_c.h>

#include <nexilis/client/protocol/af_unix/stream_client.hh>
#include <nexilis/server/protocol/af_unix/stream_server.hh>

struct nexilis_server_SettingsC
{
    nexilis::server::Settings* settings;
};

struct nexilis_ProtocolManagerC
{
    nexilis::ProtocolManager* manager;
};

struct nexilis_ProtocolDataC
{
    nexilis::ProtocolManager::ProtocolData* data;
};

struct nexilis_UnixStreamServer
{
    nexilis::server::af_unix::StreamServer* server;
};

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

nexilis_ProtocolManagerC* nexilis_protocol_manager_create()
{
    auto protocol_manager = new nexilis_ProtocolManagerC();
    protocol_manager->manager = new nexilis::ProtocolManager();
    return protocol_manager;
}

void nexilis_protocol_manager_destroy(nexilis_ProtocolManagerC* manager)
{
    if (manager)
    {
        if (manager->manager)
        {
            delete manager->manager;
            manager->manager = nullptr;
        }
        delete manager;
    }
}

nexilis_ProtocolDataC* nexilis_protocol_data_create(nexilis_ProtocolTypeC type)
{
    auto data = new nexilis_ProtocolDataC();

    auto protocol_data = nexilis::ProtocolManager::ProtocolData(static_cast<nexilis::Protocol::Type>(type));
    data->data = &protocol_data;

    return data;
}

void protocol_data_destroy(nexilis_ProtocolDataC* data)
{
    if (data)
    {
        if (data->data)
        {
            delete data->data;
            data->data = nullptr;
        }
        delete data;
    }
}

nexilis_ProtocolTypeC protocol_data_get_type(const nexilis_ProtocolDataC* data)
{
    if (data)
    {
        return static_cast<nexilis_ProtocolTypeC>(data->data->getType());
    }
    return PROTOCOL_TYPE_UNKNOWN;
}

nexilis_ProtocolStatusC protocol_data_get_status(const nexilis_ProtocolDataC* data)
{
    if (data)
    {
        return static_cast<nexilis_ProtocolStatusC>(data->data->getStatus());
    }
    return PROTOCOL_STATUS_UNDEFINED;
}

size_t protocol_data_get_id(const nexilis_ProtocolDataC* data)
{
    if (data)
    {
        return data->data->getId();
    }
    return 0;
}

nexilis_UnixStreamServer* nexilis_protocol_create_unix_stream_server(nexilis_ProtocolManagerC* manager, nexilis_server_SettingsC* settings, const char* socket_path)
{
    if (manager && manager->manager && settings && settings->settings)
    {
        auto server = new nexilis_UnixStreamServer();
        server->server = new nexilis::server::af_unix::StreamServer(
                manager->manager->createProtocol<nexilis::server::af_unix::StreamServer>(*settings->settings, socket_path));
        return server;
    }
    return nullptr;
}

void nexilis_protocol_unix_stream_server_destroy(nexilis_UnixStreamServer* server)
{
    if (server)
    {
        if (server->server)
        {
            delete server->server;
            server->server = nullptr;
        }
        delete server;
    }
}
