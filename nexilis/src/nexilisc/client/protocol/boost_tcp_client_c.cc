#include <nexilisc/client/protocol/boost_tcp_client_c.h>
#include <nexilis/client/protocol/nxboost/tcp_client.hh>

struct nexilis_BoostTCPClient
{
    nexilis::client::nxboost::TCPClient* client;
};

nexilis_BoostTCPClient* nexilis_create_boost_tcp_client(nexilis_ProtocolManagerC* manager, nexilis_ClientAPI* client_api)
{
    if (manager && manager->manager && client_api && client_api->api)
    {
        auto client = new nexilis_BoostTCPClient();
        client->client = new nexilis::client::nxboost::TCPClient(
                manager->manager->createProtocol<nexilis::client::nxboost::TCPClient>(*client_api->api));
        return client;
    }
    return nullptr;
}

void nexilis_boost_tcp_client_destroy(nexilis_BoostTCPClient* client)
{
    if (client)
    {
        if (client->client)
        {
            delete client->client;
        }
        delete client;
    }
}

nexilis_ProtocolTypeC nexilis_boost_tcp_client_get_type(nexilis_BoostTCPClient* client)
{
    if (client && client->client)
    {
        return static_cast<nexilis_ProtocolTypeC>(client->client->getType());
    }
    return PROTOCOL_TYPE_UNKNOWN;
}

void nexilis_boost_tcp_client_start(nexilis_BoostTCPClient* client)
{
    if (client && client->client)
    {
        client->client->start();
    }
}

void nexilis_boost_tcp_client_stop(nexilis_BoostTCPClient* client)
{
    if (client && client->client)
    {
        client->client->stop();
    }
}

void nexilis_boost_tcp_client_send_message(nexilis_BoostTCPClient* client, const uint8_t message[], size_t message_size)
{
    if (client && client->client)
    {
        nexilis::nx_data data;
        for (size_t i = 0; i < message_size; i++)
        {
            data.emplace_back(message[i]);
        }
        client->client->sendMessage(data);
    }
}
