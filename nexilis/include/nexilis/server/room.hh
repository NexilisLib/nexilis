#ifndef NEXILIS_ROOM_HH
#define NEXILIS_ROOM_HH

#include <nexilis/server/user.hh>
#include <nexilis/room_data.hh>
#include <nexilis/nexilis_macros.hh>

namespace nexilis::server
{

/// Nexilis Server-side API.
/// Room objects to be stored in the RoomStorage.
class Room
{
public:
    /// Constructor.
    Room(const RoomData& settings);

    /// Move constructor.
    Room(Room&& other);

    /// Move assignment operator.
    Room& operator=(Room&& other);

    /// Deleted copy constructor.
    Room(const Room&) = delete;

    /// Deleted copy assignment operator.
    Room& operator=(const Room&) = delete;

    /// Get the given name for the room.
    std::string getName() const
    {
        return m_data.getName();
    }

    /// Get the maximum amount of players in a room.
    uint32_t getMaxSize() const
    {
        return m_data.getMaxSize();
    }

    /// Get the identifier of the room.
    uint64_t getId() const
    {
        return m_data.getId();
    }

    /// Get the identifier of the creator that created the room.
    uint64_t getCreatorId() const
    {
        return m_data.getCreatorId();
    }

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
    RoomData m_data;
    std::vector<uint64_t> m_clientIds;
};

} // namespace nexilis::server

#endif
