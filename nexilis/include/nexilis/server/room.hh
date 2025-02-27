#ifndef NEXILIS_ROOM_HH
#define NEXILIS_ROOM_HH

#include <nexilis/base_room.hh>
#include <nexilis/object/object2d.hh>
#include <nexilis/object/object3d.hh>
#include <nexilis/room_data.hh>
#include <nexilis/server/user.hh>

namespace nexilis::server
{

/// Room objects to be stored in the RoomStorage.
class Room : public BaseRoom
{
public:
    /// Constructor.
    Room(const RoomData& roomData);

    /// Move constructor.
    Room(Room&& other);

    /// Move assignment operator.
    Room& operator=(Room&& other);

    /// Deleted copy constructor.
    Room(const Room&) = delete;

    /// Deleted copy assignment operator.
    Room& operator=(const Room&) = delete;

    /// User joins the room context.
    /// \param userId The id of the user that joins the room.
    void joinRoom(uint64_t userId);

    /// User leaves the room.
    /// \param userId The id the user that leaves the room.
    void leaveRoom(uint64_t userId);

    /// If the room contains certain user.
    /// \param userId The id of the user we are checking.
    bool contains(uint64_t userId);

    std::vector<uint64_t>& getClients()
    {
        return m_clientIds;
    }

    const std::vector<uint64_t>& getClients() const
    {
        return m_clientIds;
    }

private:
    std::vector<uint64_t> m_clientIds;
};

} // namespace nexilis::server

#endif
