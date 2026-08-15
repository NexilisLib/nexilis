#ifndef NEXILIS_SERVER_ROOM_HH
#define NEXILIS_SERVER_ROOM_HH

#include <nexilis/base_room.hh>
#include <nexilis/object/object_2d.hh>
#include <nexilis/object/object_3d.hh>
#include <nexilis/room_data.hh>
#include <nexilis/server/user.hh>

namespace nexilis::server
{

/// Room objects to be stored in the RoomStorage.
class Room : public BaseRoom
{
public:
    /// A broadcast message that has been sent in the room.
    struct Broadcast
    {
        /// The id of the user that sent the message.
        uint64_t clientId = 0;
        /// The payload of the message.
        std::string message = "";
    };

    /// Constructor.
    explicit Room(const RoomData& roomData);

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

    /// Store a broadcast message in the room.
    /// \param clientId The id of the user that sent the message.
    /// \param message The payload of the message.
    void addBroadcast(uint64_t clientId, const std::string& message);

    /// Get all broadcast messages that have been sent in the room.
    const std::vector<Broadcast>& getBroadcasts() const
    {
        return m_broadcasts;
    }

private:
    std::vector<uint64_t> m_clientIds;
    std::vector<Broadcast> m_broadcasts;
};

} // namespace nexilis::server

#endif
