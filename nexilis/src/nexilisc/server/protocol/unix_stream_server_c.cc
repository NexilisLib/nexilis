#ifdef __linux__

#include <nexilis/server/protocol/af_unix/stream_server.hh>
#include <nexilisc/server/protocol/unix_stream_server_c.h>

struct nexilis_UnixStreamServer
{
    nexilis::server::af_unix::StreamServer* server;
};

nexilis_UnixStreamServer* nexilis_create_unix_stream_server(nexilis_ProtocolManagerC* manager, nexilis_server_ConfigC* config, const char* socket_path)
{
    if (manager && manager->manager && config && config->settings)
    {
        auto server = new nexilis_UnixStreamServer();
        server->server = new nexilis::server::af_unix::StreamServer(
                manager->manager->createProtocol<nexilis::server::af_unix::StreamServer>(*config->settings, socket_path));
        return server;
    }
    return nullptr;
}

void nexilis_unix_stream_server_destroy(nexilis_UnixStreamServer* server)
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

nexilis_server_ConfigC nexilis_unix_stream_server_get_settings(nexilis_UnixStreamServer* server)
{
    auto config = nexilis_server_ConfigC();
    if (server && server->server)
    {
        config.settings = &server->server->getSettings();
    }
    return config;
}

nexilis_ProtocolTypeC nexilis_unix_stream_server_get_type(nexilis_UnixStreamServer* server)
{
    if (server && server->server)
    {
        return static_cast<nexilis_ProtocolTypeC>(server->server->getType());
    }
    return PROTOCOL_TYPE_UNKNOWN;
}

#endif
