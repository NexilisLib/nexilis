#ifndef NEXILISC_ROOM_DATA_C_H
#define NEXILISC_ROOM_DATA_C_H

#include <nexilisc/room_context_c.h>

#include <nexilis/room_data.hh>

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct nexilis_RoomData
{
    nexilis::RoomData* data;
};

nexilis_RoomData* nexilis_room_data_create(uint64_t creator_id, const char* name, nexilis_RoomContext context, uint32_t max_size);
void nexilis_room_data_destroy(nexilis_RoomData* room_data);

const char* nexilis_room_data_get_name(const nexilis_RoomData* room_data);
nexilis_RoomContext nexilis_room_data_get_context(const nexilis_RoomData* room_data);
uint32_t nexilis_room_data_get_max_size(const nexilis_RoomData* room_data);
uint64_t nexilis_room_data_get_id(const nexilis_RoomData* room_data);
uint64_t nexilis_room_data_get_creator_id(const nexilis_RoomData* room_data);

#ifdef __cplusplus
}
#endif

#endif
