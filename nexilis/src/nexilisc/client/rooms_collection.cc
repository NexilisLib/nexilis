#include <nexilisc/client/rooms_collection.h>

size_t nexilis_rooms_collection_rooms_count(const nexilis_RoomsCollection* collection)
{
    if (!collection || !collection->rooms)
    {
        return 0;
    }
    return collection->rooms->size();
}

const nexilis_ConstRoom* nexilis_rooms_collection_rooms_get(const nexilis_RoomsCollection* collection, size_t index)
{
    if (!collection || !collection->rooms || index >= collection->rooms->size())
    {
        return nullptr;
    }

    // Static storage to avoid allocation.
    static thread_local nexilis_ConstRoom room_wrapper;
    room_wrapper.room = &(*collection->rooms)[index];
    return &room_wrapper;
}

void nexilis_rooms_collection_free(nexilis_RoomsCollection* collection)
{
    if (collection)
    {
        delete collection;
    }
}
