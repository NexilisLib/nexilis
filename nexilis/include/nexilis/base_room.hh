#ifndef NEXILIS_BASE_ROOM_HH
#define NEXILIS_BASE_ROOM_HH

#include <nexilis/object/object_2d.hh>
#include <nexilis/object/object_3d.hh>
#include <nexilis/room_data.hh>

namespace nexilis
{

class BaseRoom
{
public:
    /// Constructor.
    explicit BaseRoom(const RoomData& roomData);

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

    /// Add an Object2D to the room.
    void addObject(Object2D&& object);

    /// Add an Object3D to the room.
    void addObject(Object3D&& object);

    // Get all 2D objects.
    const std::vector<Object2D>& getObjects2D() const;

    // Get all 3D objects.
    const std::vector<Object3D>& getObjects3D() const;

    Object2D* getObject2DById(uint64_t id);

    void deleteObject2D(uint64_t id);

    Object3D* getObject3DById(uint64_t id);

    void deleteObject3D(uint64_t id);

private:
    RoomData m_roomData;
    std::vector<Object2D> m_objects2D;
    std::vector<Object3D> m_objects3D;
};

} // namespace nexilis

#endif
