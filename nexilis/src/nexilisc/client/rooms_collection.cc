#include <nexilisc/client/rooms_collection.h>

size_t nexilis_rooms_collection_rooms_count(const nexilis_RoomsCollection* collection)
{
    return collection ? collection->rooms->size() : 0;
}

const nexilis_ConstRoom* nexilis_rooms_collection_rooms_get(const nexilis_RoomsCollection* collection, size_t index)
{
    if (!collection || index >= collection->rooms->size())
    {
        return nullptr;
    }

    // Static storage to avoid allocation.
    static thread_local nexilis_ConstRoom room_wrapper;
    room_wrapper.room = &(*collection->rooms)[index];
    return &room_wrapper;
}
