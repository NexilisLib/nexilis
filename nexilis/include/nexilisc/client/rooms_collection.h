#ifndef NEXILIS_CLIENT_ROOM_COLLECTION_H
#define NEXILIS_CLIENT_ROOM_COLLECTION_H

#include <nexilisc/client/room_c.h>

#include <nexilis/client/room.hh>

#ifdef __cplusplus
extern "C" {
#endif

struct nexilis_RoomsCollection
{
    const std::vector<nexilis::client::Room>* rooms;
};

size_t nexilis_rooms_collection_rooms_count(const nexilis_RoomsCollection* collection);
const nexilis_ConstRoom* nexilis_rooms_collection_rooms_get(const nexilis_RoomsCollection* collection, size_t index);

#ifdef __cplusplus
}
#endif

#endif
