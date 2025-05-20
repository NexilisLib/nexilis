#include <nexilis/client/room.hh>
#include <nexilisc/client/client_api_c.h>
#include <nexilisc/client/room_c.h>

struct nexilis_ClientSession
{
    nexilis::client::ClientSession* session;
};

struct nexilis_Communication
{
    nexilis::client::Room::Communication* communication;
};

nexilis_Room* nexilis_room_create(nexilis_ClientSession** clients, size_t num_clients, uint64_t creator_id, const char* name, nexilis_RoomContext context, uint32_t max_size)
{
    std::vector<nexilis::client::ClientSession> client_sessions;
    for (size_t i = 0; i < num_clients; ++i)
    {
        client_sessions.emplace_back(std::move(*clients[i]->session));
    }
    uint64_t room_id = nexilis::Util::getRandomUint64();
    auto room_data = new nexilis_RoomData();
    room_data->data = new nexilis::RoomData(creator_id, std::string(name), room_id, static_cast<nexilis::RoomData::Context>(context), max_size);

    auto room = new nexilis_Room();
    room->room = new nexilis::client::Room(*room_data->data, std::move(client_sessions));

    delete room_data;
    delete room_data->data;
    return room;
}

void nexilis_room_destroy(nexilis_Room* room)
{
    if (room)
    {
        if (room->room)
        {
            delete room->room;
            room->room = nullptr;
        }
        delete room;
    }
}

uint64_t nexilis_room_get_id(nexilis_ConstRoom* room)
{
    if (room && room->room)
    {
        return room->room->getId();
    }
    return 0;
}

void nexilis_room_add_client(nexilis_Room* room, nexilis_ClientSession* client)
{
    if (room && room->room && client && client->session)
    {
        room->room->addClient(std::move(*client->session));
    }
}

void nexilis_room_remove_client(nexilis_Room* room, uint64_t client_id)
{
    if (room && room->room)
    {
        room->room->removeClient(client_id);
    }
}

nexilis_ClientSession** nexilis_room_get_clients(nexilis_ClientAPI* client_api, const nexilis_Room* room, size_t* num_clients)
{
    if (client_api && client_api->api && room && room->room && num_clients)
    {
        const auto& clients = room->room->getClients();
        *num_clients = clients.size();
        nexilis_ClientSession** client_array = (nexilis_ClientSession**)malloc(sizeof(nexilis_ClientSession*) * (*num_clients));
        for (size_t i = 0; i < *num_clients; ++i)
        {
            client_array[i]->session = new nexilis::client::ClientSession(clients[i].getId(), client_api->api);
        }
        return client_array;
    }
    return nullptr;
}

void nexilis_room_add_message(nexilis_Room* room, nexilis_Communication* communication)
{
    if (room && room->room && communication && communication->communication)
    {
        room->room->addMessage(std::move(*communication->communication));
    }
}

nexilis_Communication** nexilis_room_get_messages(const nexilis_Room* room, size_t* num_messages)
{
    if (room && room->room && num_messages)
    {
        const auto& messages = room->room->getMessages();
        *num_messages = messages.size();
        nexilis_Communication** message_array = (nexilis_Communication**)malloc(sizeof(nexilis_Communication*) * (*num_messages));
        for (size_t i = 0; i < *num_messages; ++i)
        {
            message_array[i]->communication = new nexilis::client::Room::Communication(messages[i]);
        }
        return message_array;
    }
    return nullptr;
}

bool nexilis_room_contains_communication(const nexilis_Room* room, const nexilis_Communication* communication)
{
    if (room && room->room && communication && communication->communication)
    {
        return room->room->containsCommunication(*communication->communication);
    }
    assert(!"Undefined room");
}

bool nexilis_room_contains_communication_by_id(const nexilis_Room* room, uint64_t communication_id)
{
    if (room && room->room)
    {
        return room->room->containsCommunication(communication_id);
    }
    assert(!"Undefined room");
}

nexilis_Communication* nexilis_communication_create(const char* payload, nexilis_ClientSession* sender)
{
    if (sender && sender->session && payload)
    {
        auto communication = new nexilis_Communication();
        auto cpp_communication = new nexilis::client::Room::Communication(payload, sender->session);
        communication->communication = cpp_communication;
        return communication;
    }
    return nullptr;
}

void nexilis_communication_destroy(nexilis_Communication* communication)
{
    if (communication)
    {
        if (communication->communication)
        {
            delete communication->communication;
            communication->communication = nullptr;
        }
        delete communication;
    }
}

const char* nexilis_communication_get_payload(const nexilis_Communication* communication)
{
    if (communication && communication->communication)
    {
        std::string payload = communication->communication->getPayload();
        char* cstr = (char*)malloc(payload.size() + 1);
        if (cstr)
        {
            std::strcpy(cstr, payload.c_str());
        }
        return cstr;
    }
    return nullptr;
}

const nexilis_ClientSession* nexilis_communication_get_client(const nexilis_Communication* communication)
{
    if (communication && communication->communication)
    {
        auto client_session = new nexilis_ClientSession();
        auto cpp_client_session = communication->communication->getClient();
        client_session->session = const_cast<nexilis::client::ClientSession*>(cpp_client_session);
        return client_session;
    }
    return nullptr;
}

uint64_t nexilis_communication_get_id(const nexilis_Communication* communication)
{
    if (communication && communication->communication)
    {
        return communication->communication->getId();
    }
    return 0;
}
