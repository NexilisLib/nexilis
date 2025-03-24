#include <nexilis/server/protocol/nxboost/tcp_server.hh>
#include <nexilisc/server/protocol/boost_tcp_server_c.h>

struct nexilis_BoostTCPServer
{
    nexilis::server::nxboost::TCPServer* server;
};

nexilis_BoostTCPServer* nexilis_create_boost_tcp_server(nexilis_ProtocolManagerC* manager, nexilis_server_SettingsC* settings)
{
    if (manager && manager->manager && settings && settings->settings)
    {
        auto server = new nexilis_BoostTCPServer();
        server->server = new nexilis::server::nxboost::TCPServer(
                manager->manager->createProtocol<nexilis::server::nxboost::TCPServer>(*settings->settings));
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

nexilis_server_SettingsC nexilis_boost_tcp_server_get_settings(nexilis_BoostTCPServer* server)
{
    auto settings = nexilis_server_SettingsC();
    if (server && server->server)
    {
        settings.settings = &server->server->getSettings();
    }
    return settings;
}

nexilis_ProtocolTypeC nexilis_boost_tcp_server_get_type(nexilis_BoostTCPServer* server)
{
    if (server && server->server)
    {
        return static_cast<nexilis_ProtocolTypeC>(server->server->getType());
    }
    return PROTOCOL_TYPE_UNKNOWN;
}
