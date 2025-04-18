#include <nexilis/client/packet.hh>
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
    return convert_nx_data(nexilis::client::Packet::Info::rooms());
}