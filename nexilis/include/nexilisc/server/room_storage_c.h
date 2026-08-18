#ifndef NEXILISC_SERVER_ROOM_STORAGE_C_H
#define NEXILISC_SERVER_ROOM_STORAGE_C_H

#include <nexilisc/room_data_c.h>
#include <nexilis/server/room_storage.hh>

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

struct nexilis_ServerRoom
{
    nexilis::server::Room* room;
};

void nexilis_server_room_storage_add(nexilis_RoomData* room_data);
bool nexilis_server_room_storage_contains(uint64_t id);
nexilis_ServerRoom* nexilis_server_room_storage_get_room_by_id(uint64_t id);
uint64_t nexilis_server_room_storage_get_all_rooms_count();
nexilis_ServerRoom* nexilis_server_room_storage_get_room_at(uint64_t index);
void nexilis_server_room_storage_clear();

// Room operations
void nexilis_server_room_join(nexilis_ServerRoom* room, uint64_t user_id);
void nexilis_server_room_leave(nexilis_ServerRoom* room, uint64_t user_id);
bool nexilis_server_room_contains(nexilis_ServerRoom* room, uint64_t user_id);
uint64_t nexilis_server_room_get_client_count(nexilis_ServerRoom* room);

#ifdef __cplusplus
}
#endif

#endif
