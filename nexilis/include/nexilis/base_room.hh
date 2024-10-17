#ifndef NEXILIS_BASE_ROOM_HH
#define NEXILIS_BASE_ROOM_HH

#include <nexilis/room_data.hh>
#include <nexilis/object/object2d.hh>
#include <nexilis/object/object3d.hh>

#include <variant>

namespace nexilis
{

using RoomObjects = std::variant<std::vector<Object2D>, std::vector<Object3D>>;

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

    /// Get the 2D/3D context for the room.
    RoomData::Context getContext() const
    {
        return m_roomData.getContext();
    }

    /// Add an Object2D to the room (switch to 2D mode)
    void addObject(Object2D&& object)
    {
        if (!std::holds_alternative<std::vector<Object2D>>(m_objects)) {
            // If m_objects doesn't currently hold Object2D, clear and switch to Object2D
            m_objects = std::vector<Object2D>{};
        }
        std::get<std::vector<Object2D>>(m_objects).emplace_back(std::move(object));
    }

    /// Add an Object3D to the room (switch to 3D mode)
    void addObject(Object3D&& object)
    {
        if (!std::holds_alternative<std::vector<Object3D>>(m_objects)) {
            // If m_objects doesn't currently hold Object3D, clear and switch to Object3D
            m_objects = std::vector<Object3D>{};
        }
        std::get<std::vector<Object3D>>(m_objects).emplace_back(std::move(object));
    }

    /// Get mutable reference to the objects (Object2D or Object3D)
    RoomObjects& getObjects()
    {
        return m_objects;
    }

    /// Get const reference to the objects (Object2D or Object3D)
    const RoomObjects& getObjects() const
    {
        return m_objects;
    }

private:
    RoomData m_roomData;
    RoomObjects m_objects;
};

}

#endif