#ifndef NEXILIS_ROOM_HH
#define NEXILIS_ROOM_HH

#include <cstdint>
#include <nexilis/common/util.hh>
#include <nexilis/nexilis_macros.hh>

namespace nexilis
{

/// Room objects to be stored in the RoomStorage.
class Room
{
public:
    /// The settings for the associated room.
    class Settings
    {
    public:
        /// Constructor.
        Settings(const std::string& name, uint32_t maxSize);

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

    private:
        std::string m_name;
        uint32_t m_maxSize = NEXILIS_ROOM_CLIENT_AMOUNT;
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

private:
    Settings m_settings;
};

}

#endif
