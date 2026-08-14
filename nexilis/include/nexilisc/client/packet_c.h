#ifndef NEXILISC_CLIENT_PACKET_C_H
#define NEXILISC_CLIENT_PACKET_C_H

#include <nexilisc/nx_data_c.h>
#include <nexilisc/room_context_c.h>

#include <nexilisc/types/vector3_c.h>

struct nexilis_ClientAPI;

#ifdef __cplusplus
extern "C"
{
#endif

    // Info messages
    nx_data_c nexilis_packet_get_info_general(nexilis_ClientAPI* client_api);
    nx_data_c nexilis_packet_get_info_clients(nexilis_ClientAPI* client_api);
    nx_data_c nexilis_packet_get_info_rooms(nexilis_ClientAPI* client_api);

    // Room management
    nx_data_c nexilis_packet_room_management_join(nexilis_ClientAPI* client_api, uint64_t room_id);
    nx_data_c nexilis_packet_room_management_leave(nexilis_ClientAPI* client_api);
    nx_data_c nexilis_packet_room_management_create(nexilis_ClientAPI* client_api, const char* room_name, nexilis_RoomContext ctx = ROOM_CONTEXT_3D);

    // Room player3D
    nx_data_c nexilis_packet_room_player3D_position(nexilis_ClientAPI* client_api, nexilis_Vector3f* position);
    nx_data_c nexilis_packet_room_player3D_position_direct(nexilis_ClientAPI* client_api, float x, float y, float z);
    nx_data_c nexilis_packet_room_player3D_dimension(nexilis_ClientAPI* client_api, nexilis_Vector3f* dimensions);
    nx_data_c nexilis_packet_room_player3D_movement(nexilis_ClientAPI* client_api, nexilis_Vector3f* movement, float deltaTime);
    nx_data_c nexilis_packet_room_player3D_movement_direct(nexilis_ClientAPI* client_api, float x, float y, float z, float deltaTime);

#ifdef __cplusplus
}
#endif

#endif
