#ifndef NEXILIS_CLIENT_ROOM_COLLECTION_H
#define NEXILIS_CLIENT_ROOM_COLLECTION_H

#include <nexilisc/client/room_c.h>

#include <nexilis/client/room.hh>

#ifdef __cplusplus
extern "C" {
#endif

struct nexilis_RoomsCollection
{
    std::vector<nexilis::client::Room>* rooms;
};

struct nexilis_RoomsCollectionClients
{
    const std::vector<nexilis::client::ClientSession>* clients;
};

size_t nexilis_rooms_collection_rooms_count(const nexilis_RoomsCollection* collection);
nexilis_Room* nexilis_rooms_collection_room_get(nexilis_RoomsCollection* collection, size_t index);
void nexilis_rooms_collection_free(nexilis_RoomsCollection* collection);

#ifdef __cplusplus
}
#endif

#endif
