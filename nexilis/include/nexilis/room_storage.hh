#ifndef NEXILIS_ROOM_STORAGE_HH
#define NEXILIS_ROOM_STORAGE_HH

#include <nexilis/room.hh>

#include <vector>

namespace nexilis
{

/// Nexilis-Server side API.
/// Creating static lifetime for the rooms in the server context.
class RoomStorage
{
public:
    /// Constructor.
    RoomStorage() = default;

    /// Add new room to the server.
    static void add(Room&& room);

    /// Check if room exists.
    /// \param id The id of the room.
    static bool contains(uint64_t id);

    /// Get all the rooms in the server.
    static std::vector<Room>& getAllRooms();

    /// Get pointer of the room.
    /// \param id The id of the room.
    static Room* getRoomById(uint64_t id);

private:
    static std::vector<Room> m_rooms;
};

} // namespace nexilis

#endif
