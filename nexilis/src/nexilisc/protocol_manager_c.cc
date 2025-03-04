#include <nexilis/protocol_manager.hh>
#include <nexilisc/protocol_manager_c.h>

#include <nexilis/client/protocol/af_unix/stream_client.hh>
#include <nexilis/server/protocol/af_unix/stream_server.hh>

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

void nexilis_protocol_data_destroy(nexilis_ProtocolDataC* data)
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

nexilis_ProtocolTypeC nexilis_protocol_data_get_type(const nexilis_ProtocolDataC* data)
{
    if (data)
    {
        return static_cast<nexilis_ProtocolTypeC>(data->data->getType());
    }
    return PROTOCOL_TYPE_UNKNOWN;
}

nexilis_ProtocolStatusC nexilis_protocol_data_get_status(const nexilis_ProtocolDataC* data)
{
    if (data)
    {
        return static_cast<nexilis_ProtocolStatusC>(data->data->getStatus());
    }
    return PROTOCOL_STATUS_UNDEFINED;
}

size_t nexilis_protocol_data_get_id(const nexilis_ProtocolDataC* data)
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
