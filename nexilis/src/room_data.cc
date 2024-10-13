#include <nexilis/room_data.hh>

namespace nexilis
{

RoomData::RoomData(uint64_t creatorId, const std::string& name, uint32_t maxSize)
    : m_creatorId(creatorId),
      m_name(name),
      m_maxSize(maxSize)
{
}

RoomData::RoomData(const RoomData& other)
    : m_creatorId(other.m_creatorId),
      m_name(other.m_name),
      m_maxSize(other.m_maxSize),
      m_roomId(other.m_roomId)
{
}

RoomData::RoomData(RoomData&& other)
    : m_creatorId(std::move(other.m_creatorId)),
      m_name(std::move(other.m_name)),
      m_maxSize(std::move(other.m_maxSize)),
      m_roomId(std::move(other.m_roomId))
{
}

RoomData& RoomData::operator=(const RoomData& other)
{
    if (this != &other)
    {
        m_creatorId = other.m_creatorId;
        m_name = other.m_name;
        m_maxSize = other.m_maxSize;
        m_roomId = other.m_roomId;
    }
    return *this;
}

RoomData& RoomData::operator=(RoomData&& other)
{
    if (this != &other)
    {
        m_creatorId = std::move(other.m_creatorId);
        m_name = std::move(other.m_name);
        m_maxSize = std::move(other.m_maxSize);
        m_roomId = std::move(other.m_roomId);
    }
    return *this;
}

}