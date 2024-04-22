#ifndef NEXILIS_ROOM_HH
#define NEXILIS_ROOM_HH

#include <cstdint>
#include <nexilis/common/util.hh>
#include <nexilis/nexilis_macros.hh>

namespace nexilis
{

/// Nexilis Server-side API.
/// Room objects to be stored in the RoomStorage.
class Room
{
public:
    /// The settings for the associated room.
    class Settings
    {
    public:
        /// Constructor.
        /// \param creatorId The identifier of the creator.
        /// \param name 1-15 characters of text for the name of the room.
        /// \param maxSize The maximum size of the room.
        Settings(uint64_t creatorId, const std::string& name, uint32_t maxSize = NEXILIS_ROOM_CLIENT_AMOUNT);

        /// Copy constructor.
        Settings(const Settings& other);

        /// Move constructor.
        Settings(Settings&& other);

        /// Copy assignment operator.
        Settings& operator=(const Settings& other);

        /// Move assignment operator.
        Settings& operator=(Settings&& other);

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
        uint64_t m_creatorId;
        std::string m_name;
        uint32_t m_maxSize;
        uint64_t m_roomId = Util::getRandomUint64();
    };

    /// Constructor.
    Room(const Settings& settings);

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
        return m_settings.getName();
    }

    /// Get the maximum amount of players in a room.
    uint32_t getMaxSize() const
    {
        return m_settings.getMaxSize();
    }

    /// Get the identifier of the room.
    uint64_t getId() const
    {
        return m_settings.getId();
    }

    /// Get the identifier of the creator that created the room.
    uint64_t getCreatorId() const
    {
        return m_settings.getCreatorId();
    }

    //std::string getCreatorName() const;

    /// User joins the room context.
    /// \param userId The identifier of the user.
    void joinRoom(uint64_t userId);

    /// User leaves the room.
    /// \param userId The identifier of the user.
    void leaveRoom(uint64_t userId);

    /// If the room contains certiain client.
    /// \param userId The identifier of the user.
    bool contains(uint64_t userId);

private:
    Settings m_settings;

    /// Users inside the room.
    std::vector<uint64_t> m_users;
};

} // namespace nexilis

#endif
