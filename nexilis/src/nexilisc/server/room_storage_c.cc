#include <nexilisc/server/room_storage_c.h>

#include <nexilis/server/room.hh>
#include <nexilis/server/room_storage.hh>

void nexilis_server_room_storage_add(nexilis_RoomData* room_data)
{
    if (room_data && room_data->data)
    {
        nexilis::server::Room room(*room_data->data);
        nexilis::server::RoomStorage::add(std::move(room));
    }
}

bool nexilis_server_room_storage_contains(uint64_t id)
{
    return nexilis::server::RoomStorage::contains(id);
}

nexilis_ServerRoom* nexilis_server_room_storage_get_room_by_id(uint64_t id)
{
    auto room = nexilis::server::RoomStorage::getRoomById(id);
    if (room)
    {
        auto wrapper = new nexilis_ServerRoom();
        wrapper->room = room;
        return wrapper;
    }
    return nullptr;
}

uint64_t nexilis_server_room_storage_get_all_rooms_count()
{
    return nexilis::server::RoomStorage::getAllRooms().size();
}

nexilis_ServerRoom* nexilis_server_room_storage_get_room_at(uint64_t index)
{
    auto& rooms = nexilis::server::RoomStorage::getAllRooms();
    if (index < rooms.size())
    {
        auto wrapper = new nexilis_ServerRoom();
        wrapper->room = &rooms[index];
        return wrapper;
    }
    return nullptr;
}

void nexilis_server_room_storage_clear()
{
    nexilis::server::RoomStorage::clear();
}

void nexilis_server_room_join(nexilis_ServerRoom* room, uint64_t user_id)
{
    if (room && room->room)
    {
        room->room->joinRoom(user_id);
    }
}

void nexilis_server_room_leave(nexilis_ServerRoom* room, uint64_t user_id)
{
    if (room && room->room)
    {
        room->room->leaveRoom(user_id);
    }
}

bool nexilis_server_room_contains(nexilis_ServerRoom* room, uint64_t user_id)
{
    if (room && room->room)
    {
        return room->room->contains(user_id);
    }
    return false;
}

uint64_t nexilis_server_room_get_client_count(nexilis_ServerRoom* room)
{
    if (room && room->room)
    {
        return room->room->getClients().size();
    }
    return 0;
}
