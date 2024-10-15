#ifndef NEXILIS_BASE_ROOM_HH
#define NEXILIS_BASE_ROOM_HH

#include <nexilis/room_data.hh>

namespace nexilis
{

class BaseRoom
{
public:
    /// Constructor.
    BaseRoom(const RoomData& roomData);

    /// Move constructor.
    BaseRoom(BaseRoom&& other);

    /// Move assignment operator.
    BaseRoom& operator=(BaseRoom&& other);

    /// Deleted copy constructor.
    BaseRoom(const BaseRoom&) = delete;

    /// Deleted copy assignment operator.
    BaseRoom& operator=(const BaseRoom&) = delete;

    /// Get given name for the room.
    std::string getName() const
    {
        return m_roomData.getName();
    }

    /// Get the maximum amount of players in a room.
    uint32_t getMaxSize() const
    {
        return m_roomData.getMaxSize();
    }

    /// Get the identifier of the room.
    uint64_t getId() const
    {
        return m_roomData.getId();
    }

    /// Get the identifier of the creator that created the room.
    uint64_t getCreatorId() const
    {
        return m_roomData.getCreatorId();
    }

private:
    RoomData m_roomData;
};

}

#endif