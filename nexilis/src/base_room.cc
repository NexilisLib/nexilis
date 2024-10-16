#include <nexilis/base_room.hh>

namespace nexilis
{

BaseRoom::BaseRoom(const RoomData& roomData)
    : m_roomData(roomData)
{
}

BaseRoom::BaseRoom(BaseRoom&& other) :
    m_roomData(std::move(other.m_roomData)),
    m_objects(std::move(other.m_objects))
{
}

BaseRoom& BaseRoom::operator=(BaseRoom&& other)
{
    if (this != &other)
    {
        m_roomData = std::move(other.m_roomData);
        m_objects = std::move(other.m_objects);
    }
    return *this;
}

}
