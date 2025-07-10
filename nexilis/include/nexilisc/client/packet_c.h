#ifndef NEXILISC_CLIENT_PACKET_C_H
#define NEXILISC_CLIENT_PACKET_C_H

#include <nexilisc/nx_data_c.h>
#include <nexilisc/room_context_c.h>

#include <nexilisc/types/vector3_c.h>

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

// Room player3D
nx_data_c nexilis_packet_room_player3D_position(nexilis_Vector3f* position);
nx_data_c nexilis_packet_room_player3D_position_direct(float x, float y, float z);
nx_data_c nexilis_packet_room_player3D_dimensions(nexilis_Vector3f* dimensions);
nx_data_c nexilis_packet_room_player3D_movement(nexilis_Vector3f* movement, float deltaTime);
nx_data_c nexilis_packet_room_player3D_movement_direct(float x, float y, float z, float deltaTime);

#ifdef __cplusplus
}
#endif

#endif
