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

uint64_t nexilis_client_session_get_id(nexilis_ClientSession* session)
{
    if (session && session->client)
    {
        return session->client->getId();
    }
    return 0;
}

void nexilis_client_session_set_position_3D(nexilis_ClientSession* client, float x, float y, float z)
{
    if (client && client->client)
    {
        client->client->setPosition3D(x, y, z);
    }
}

nexilis_Vector3f* nexilis_client_session_get_position_3D(nexilis_ClientSession* client)
{
    if (!client || !client->client)
    {
        return nullptr;
    }
    try
    {
        auto* vector = new nexilis_Vector3f;
        vector->vec = new nexilis::Vector3f(client->client->getPosition3D());
        return vector;
    }
    catch (...)
    {
        nexilis::FileLog::critical("Problem with nexilis_client_session_get_position_3D");
        return nullptr;
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
