#include <nexilis/client/packet.hh>
#include <nexilis/logger/file_log.hh>
#include <nexilis/util.hh>
#include <nexilisc/client/packet_c.h>

static nx_data_c convert_nx_data(const nexilis::nx_data& data)
{
    return nexilis_nx_data_create_from(data.data(), data.size());
}

static nexilis::Vector3f convert_vector3f(nexilis_Vector3f* vector)
{
    float x = vector->vec->x;
    float y = vector->vec->y;
    float z = vector->vec->z;
    nexilis::FileLog::debug("Converted position x:", x, " y:", y, " z:", z);
    return nexilis::Vector3f(x, y, z);
}

nx_data_c nexilis_packet_room_management_join(uint64_t room_id)
{
    return convert_nx_data(nexilis::client::Packet::Room::Management::join(room_id));
}

nx_data_c nexilis_packet_room_management_leave()
{
    return convert_nx_data(nexilis::client::Packet::Room::Management::leave());
}

nx_data_c nexilis_packet_room_management_create(nexilis_RoomContext ctx, const char* room_name)
{
    return convert_nx_data(nexilis::client::Packet::Room::Management::create(
            nexilis::RoomData::Context(ctx), std::string(room_name)));
}

nx_data_c nexilis_packet_room_player3D_position(nexilis_Vector3f* position)
{
    if (!position || !position->vec)
    {
        nexilis::FileLog::error("Incorrect vector for nexilis_packet_room_player3D_position");
        return nx_data_c{};
    }
    return convert_nx_data(nexilis::client::Packet::Room::Player3D::position(convert_vector3f(position)));
}

nx_data_c nexilis_packet_room_player3D_position_direct(float x, float y, float z)
{
    return convert_nx_data(nexilis::client::Packet::Room::Player3D::position(nexilis::Vector3f(x, y, z)));
}

nx_data_c nexilis_packet_room_player3D_dimensions(nexilis_Vector3f* dimensions)
{
    return convert_nx_data(nexilis::client::Packet::Room::Player3D::dimensions(convert_vector3f(dimensions)));
}

nx_data_c nexilis_packet_room_player3D_movement(nexilis_Vector3f* movement, float deltaTime)
{
    return convert_nx_data(nexilis::client::Packet::Room::Player3D::movement(convert_vector3f(movement), deltaTime));
}

nx_data_c nexilis_packet_room_player3D_movement_direct(float x, float y, float z, float deltatime)
{
    return convert_nx_data(nexilis::client::Packet::Room::Player3D::movement(nexilis::Vector3f(x, y, z), deltatime));
}

nx_data_c nexilis_packet_info_general()
{
    return convert_nx_data(nexilis::client::Packet::Info::general());
}

nx_data_c nexilis_packet_info_clients()
{
    return convert_nx_data(nexilis::client::Packet::Info::clients());
}

nx_data_c nexilis_packet_info_rooms()
{
    return convert_nx_data(nexilis::client::Packet::Info::rooms());
}
