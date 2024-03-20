#ifndef NEXILIS_ROOM_STORAGE_HH
#define NEXILIS_ROOM_STORAGE_HH

#include <nexilis/room.hh>

#include <vector>

namespace nexilis
{

/// Creating static lifetime for the rooms in the server context.

class RoomStorage
{
public:
    /// Constructor.
    RoomStorage() = default;

    static void add(Room&& room);

    static bool contains(size_t id);

    static std::vector<Room>& getAllRooms();

    static Room* getRoomById(size_t id);

private:
    static std::vector<Room> m_rooms;
};

} // namespace nexilis

#endif
