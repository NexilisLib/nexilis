#include <nexilis/base_room.hh>
#include <nexilis/logger/log.hh>

namespace nexilis
{

BaseRoom::BaseRoom(const RoomData& roomData)
    : m_roomData(roomData)
{
}

BaseRoom::BaseRoom(BaseRoom&& other) :
    m_roomData(std::move(other.m_roomData)),
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
    if (getContext() != RoomData::Context::_2D)
    {
        Log::error("No 2D context");
    }
    m_objects2D.emplace_back(std::move(object));
}

void BaseRoom::addObject(Object3D&& object)
{
    if (getContext() != RoomData::Context::_3D)
    {
        Log::error("No 3D context");
    }
    m_objects3D.emplace_back(std::move(object));
}

const std::vector<Object2D>& BaseRoom::getObjects2D() const
{
    if (getContext() != RoomData::Context::_2D)
    {
        Log::error("No 2D context");
    }
    return m_objects2D;
}

const std::vector<Object3D>& BaseRoom::getObjects3D() const
{
    if (getContext() != RoomData::Context::_3D)
    {
        Log::error("No 3D context");
    }
    return m_objects3D;
}

Object2D* BaseRoom::getObject2DById(uint64_t id)
{
    if (getContext() != RoomData::Context::_2D)
    {
        Log::error("No 2D context");
    }

    for (auto& object : m_objects2D)
    {
        if (object.getId() == id)
        {
            return &object;
        }
    }
    return nullptr;
}

}
