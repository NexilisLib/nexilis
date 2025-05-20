#include <nexilis/client/packet.hh>
#include <nexilis/logger/file_log.hh>
#include <nexilis/util.hh>
#include <nexilisc/client/packet_c.h>

static nx_data_c convert_nx_data(const nexilis::nx_data& data)
{
    return nexilis_nx_data_create_from(data.data(), data.size());
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
    auto a = nexilis::client::Packet::Info::rooms();
    nexilis::FileLog::debug("INFO ROOMS: ", a);
    nexilis::FileLog::debug("Info rooms as numbers: ", nexilis::Util::convertToNumbers(a));
    return convert_nx_data(a);
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
