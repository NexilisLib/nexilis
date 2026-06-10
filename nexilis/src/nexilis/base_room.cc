#include <nexilis/base_room.hh>
#include <nexilis/logger/log.hh>

namespace nexilis
{

BaseRoom::BaseRoom(const RoomData& roomData)
    : m_roomData(roomData)
{
}

BaseRoom::BaseRoom(BaseRoom&& other)
    : m_roomData(std::move(other.m_roomData)),
      m_objects2D(std::move(other.m_objects2D)),
      m_objects3D(std::move(other.m_objects3D))
{
}

BaseRoom& BaseRoom::operator=(BaseRoom&& other)
{
    if (this != &other)
    {
        m_roomData = std::move(other.m_roomData);
        m_objects2D = std::move(other.m_objects2D);
        m_objects3D = std::move(other.m_objects3D);
    }
    return *this;
}

void BaseRoom::addObject(Object2D&& object)
{
    m_objects2D.emplace_back(std::move(object));
}

void BaseRoom::addObject(Object3D&& object)
{
    if (getContext() == RoomData::Context::_2D)
    {
        Log::error("Cannot add 3D object in a 2D context room");
        return;
    }
    m_objects3D.emplace_back(std::move(object));
}

const std::vector<Object2D>& BaseRoom::getObjects2D() const
{
    // 2D objects are allowed to exist in 3D context.
    return m_objects2D;
}

const std::vector<Object3D>& BaseRoom::getObjects3D() const
{
    if (getContext() == RoomData::Context::_2D)
    {
        Log::error("No 3D context");
    }
    return m_objects3D;
}

Object2D* BaseRoom::getObject2DById(uint64_t id)
{
    for (auto& object : m_objects2D)
    {
        if (object.getId() == id)
        {
            return &object;
        }
    }
    return nullptr;
}

Object3D* BaseRoom::getObject3DById(uint64_t id)
{
    if (getContext() == RoomData::Context::_2D)
    {
        Log::error("No 3D context");
        return nullptr;
    }

    for (auto& object : m_objects3D)
    {
        if (object.getId() == id)
        {
            return &object;
        }
    }
    return nullptr;
}

void BaseRoom::deleteObject2D(uint64_t objectId)
{
    auto it = std::find_if(m_objects2D.begin(), m_objects2D.end(),
                           [objectId](const Object2D& obj)
                           {
                               return obj.getId() == objectId;
                           });

    if (it != m_objects2D.end())
    {
        m_objects2D.erase(it);
        Log::info("Deleted object: ", objectId);
    }
    else
    {
        Log::info("Could not find object ", objectId, " for deletion");
    }
}

void BaseRoom::deleteObject3D(uint64_t objectId)
{
    auto it = std::find_if(m_objects3D.begin(), m_objects3D.end(),
                           [objectId](const Object3D& obj)
                           {
                               return obj.getId() == objectId;
                           });

    if (it != m_objects3D.end())
    {
        m_objects3D.erase(it);
        Log::info("Deleted object: ", objectId);
    }
    else
    {
        Log::info("Could not find object ", objectId, " for deletion");
    }
}

} // namespace nexilis
