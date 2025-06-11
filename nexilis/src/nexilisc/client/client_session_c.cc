#include <nexilis/client/client_session.hh>
#include <nexilisc/client/client_session_c.h>

nexilis_ClientSession* nexilis_client_session_create(uint64_t id, nexilis_ClientAPI* client_api)
{
    auto session = new nexilis::client::ClientSession(id, client_api->api);
    return new nexilis_ClientSession{session};
}

void nexilis_client_session_destroy(nexilis_ClientSession* client)
{
    if (client)
    {
        delete static_cast<nexilis::client::ClientSession*>(client->client);
        delete client;
    }
}

nexilis_ClientSession* nexilis_client_session_move(nexilis_ClientSession* other)
{
    if (!other)
    {
        return nullptr;
    }

    auto moved_session = new nexilis::client::ClientSession(
            std::move(*static_cast<nexilis::client::ClientSession*>(other->client)));
    return new nexilis_ClientSession{moved_session};
}

void nexilis_client_session_move_assign(nexilis_ClientSession* dest, nexilis_ClientSession* src)
{
    if (!dest || !src)
        return;

    *static_cast<nexilis::client::ClientSession*>(dest->client) =
            std::move(*static_cast<nexilis::client::ClientSession*>(src->client));
}

bool nexilis_client_session_equal(const nexilis_ClientSession* lhs, const nexilis_ClientSession* rhs)
{
    if (!lhs || !rhs)
        return false;

    return *static_cast<const nexilis::client::ClientSession*>(lhs->client) ==
           *static_cast<const nexilis::client::ClientSession*>(rhs->client);
}

bool nexilis_client_session_not_equal(const nexilis_ClientSession* lhs, const nexilis_ClientSession* rhs)
{
    return !nexilis_client_session_equal(lhs, rhs);
}

void nexilis_client_session_set_username(nexilis_ClientSession* client, const char* new_username)
{
    if (client && new_username)
    {
        static_cast<nexilis::client::ClientSession*>(client->client)->setUsername(new_username);
    }
}
