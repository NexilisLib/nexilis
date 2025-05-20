#ifndef NEXILISC_CLIENT_PACKET_C_H
#define NEXILISC_CLIENT_PACKET_C_H

#include <nexilisc/nx_data_c.h>
#include <nexilisc/room_context_c.h>

#ifdef __cplusplus
extern "C" {
#endif

// Info messages
nx_data_c nexilis_packet_info_general();
nx_data_c nexilis_packet_info_clients();
nx_data_c nexilis_packet_info_rooms();

// Room management
nx_data_c nexilis_packet_room_management_join(uint64_t room_id);
nx_data_c nexilis_packet_room_management_leave();
nx_data_c nexilis_packet_room_management_create(nexilis_RoomContext ctx, const char* room_name);

#ifdef __cplusplus
}
#endif

#endif