#include <nexilis/room_data.hh>
#include <nexilisc/room_data_c.h>

nexilis_RoomData* nexilis_room_data_create(uint64_t creator_id, const char* name, uint64_t room_id, nexilis_RoomContext context, uint32_t max_size)
{
    auto room_data = new nexilis_RoomData();
    room_data->data = new nexilis::RoomData(creator_id, name, room_id, static_cast<nexilis::RoomData::Context>(context), max_size);
    return room_data;
}

void nexilis_room_data_destroy(nexilis_RoomData* room_data)
{
    if (room_data)
    {
        if (room_data->data)
        {
            delete room_data->data;
            room_data->data = nullptr;
        }
        delete room_data;
    }
}

const char* nexilis_room_data_get_name(const nexilis_RoomData* room_data)
{
    if (room_data && room_data->data)
    {
        std::string name = room_data->data->getName();
        char* cstr = (char*)malloc(name.size() + 1);
        if (cstr)
        {
            std::strcpy(cstr, name.c_str());
        }
        return cstr;
    }
    return nullptr;
}

nexilis_RoomContext nexilis_room_data_get_context(const nexilis_RoomData* room_data)
{
    if (room_data && room_data->data)
    {
        return static_cast<nexilis_RoomContext>(room_data->data->getContext());
    }
    return ROOM_CONTEXT_UNDEFINED;
}

uint32_t nexilis_room_data_get_max_size(const nexilis_RoomData* room_data)
{
    if (room_data && room_data->data)
    {
        return room_data->data->getMaxSize();
    }
    return 0;
}

uint64_t nexilis_room_data_get_id(const nexilis_RoomData* room_data)
{
    if (room_data && room_data->data)
    {
        return room_data->data->getId();
    }
    return 0;
}

uint64_t nexilis_room_data_get_creator_id(const nexilis_RoomData* room_data)
{
    if (room_data && room_data->data)
    {
        return room_data->data->getCreatorId();
    }
    return 0;
}
