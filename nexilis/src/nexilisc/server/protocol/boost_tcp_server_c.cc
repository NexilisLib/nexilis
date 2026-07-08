#include <nexilis/server/protocol/nxboost/tcp_server.hh>
#include <nexilisc/server/protocol/boost_tcp_server_c.h>

struct nexilis_BoostTCPServer
{
    nexilis::server::nxboost::TCPServer* server;
};

nexilis_BoostTCPServer* nexilis_create_boost_tcp_server(nexilis_ProtocolManagerC* manager, nexilis_server_ConfigC* config)
{
    if (manager && manager->manager && config && config->settings)
    {
        auto server = new nexilis_BoostTCPServer();
        server->server = new nexilis::server::nxboost::TCPServer(
                manager->manager->createProtocol<nexilis::server::nxboost::TCPServer>(*config->settings));
        return server;
    }
    return nullptr;
}

void nexilis_boost_tcp_server_destroy(nexilis_BoostTCPServer* server)
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

void nexilis_boost_tcp_server_start(nexilis_BoostTCPServer* server)
{
    if (server && server->server)
    {
        server->server->start();
    }
}

void nexilis_boost_tcp_server_stop(nexilis_BoostTCPServer* server)
{
    if (server && server->server)
    {
        server->server->stop();
    }
}

nexilis_server_ConfigC nexilis_boost_tcp_server_get_settings(nexilis_BoostTCPServer* server)
{
    auto config = nexilis_server_ConfigC();
    if (server && server->server)
    {
        config.settings = &server->server->getSettings();
    }
    return config;
}

nexilis_ProtocolTypeC nexilis_boost_tcp_server_get_type(nexilis_BoostTCPServer* server)
{
    if (server && server->server)
    {
        return static_cast<nexilis_ProtocolTypeC>(server->server->getType());
    }
    return PROTOCOL_TYPE_UNKNOWN;
}
