#ifndef NEXILIS_ROOM_HH
#define NEXILIS_ROOM_HH

#include <nexilis/common/util.hh>
#include <nexilis/nexilis_macros.hh>

namespace nexilis
{

/// Nexilis Server-side API.
/// Room objects to be stored in the RoomStorage.
class Room
{
public:
    class Client
    {
    public:
        Client(uint64_t clientId, const std::string& name = "UNNAMED");

        /// Copy constructor.
        Client(const Client& other);

        /// Copy assignment operator.
        Client& operator=(const Client& other);

        /// Move constructor.
        Client(Client&& other);

        /// Move assignment operator.
        Client& operator=(Client&& other);

        uint64_t getId() const
        {
            return m_id;
        }

        std::string getName() const
        {
            return m_name;
        }

    private:
        uint64_t m_id;
        std::string m_name;
    };

    /// The data and settings for the associated room.
    class Data
    {
    public:
        /// Constructor.
        /// \param creatorId The identifier of the creator.
        /// \param name 1-15 characters of text for the name of the room.
        /// \param maxSize The maximum size of the room.
        Data(uint64_t creatorId, const std::string& name, uint32_t maxSize = NEXILIS_ROOM_CLIENT_AMOUNT);

        /// Copy constructor.
        Data(const Data& other);

        /// Move constructor.
        Data(Data&& other);

        /// Copy assignment operator.
        Data& operator=(const Data& other);

        /// Move assignment operator.
        Data& operator=(Data&& other);

        /// Get the given name for the room.
        std::string getName() const
        {
            return m_name;
        }

        /// Get the maximum amount of players in a room.
        uint32_t getMaxSize() const
        {
            return m_maxSize;
        }

        /// Get the identifier of the room.
        uint64_t getId() const
        {
            return m_roomId;
        }

        uint64_t getCreatorId() const
        {
            return m_creatorId;
        }

       private:
        /// Id of the creator of this room.
        uint64_t m_creatorId;

        /// The name of this room.
        std::string m_name;

        /// The maximum amount of players in a room.
        uint32_t m_maxSize;

        /// The unique identifier of this room.
        uint64_t m_roomId = Util::getRandomUint64();
    };

    /// Constructor.
    Room(const Data& settings);

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
    /// \param userId The identifier of the user.
    void joinRoom(uint64_t userId);

    /// User leaves the room.
    /// \param userId The identifier of the user.
    void leaveRoom(uint64_t userId);

    /// If the room contains certiain client.
    /// \param userId The identifier of the user.
    bool contains(uint64_t userId);

    std::vector<Room::Client> getClients() const
    {
        return m_clients;
    }

private:
    Data m_data;
    std::vector<Room::Client> m_clients;
};

} // namespace nexilis

#endif
