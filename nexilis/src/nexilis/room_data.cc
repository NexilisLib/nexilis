#include <nexilis/room_data.hh>

namespace nexilis
{

RoomData::RoomData(uint64_t creatorId, const std::string& name, uint64_t roomId, Context context, uint32_t maxSize)
    : m_creatorId(creatorId),
      m_name(name),
      m_roomId(roomId),
      m_context(context),
      m_maxSize(maxSize)
{
}

RoomData::RoomData(const RoomData& other)
    : m_creatorId(other.m_creatorId),
      m_name(other.m_name),
      m_roomId(other.m_roomId),
      m_context(other.m_context),
      m_maxSize(other.m_maxSize)
{
}

RoomData::RoomData(RoomData&& other)
    : m_creatorId(std::move(other.m_creatorId)),
      m_name(std::move(other.m_name)),
      m_roomId(std::move(other.m_roomId)),
      m_context(std::move(other.m_context)),
      m_maxSize(std::move(other.m_maxSize))
{
}

RoomData& RoomData::operator=(const RoomData& other)
{
    if (this != &other)
    {
        m_creatorId = other.m_creatorId;
        m_name = other.m_name;
        m_roomId = other.m_roomId;
        m_context = other.m_context;
        m_maxSize = other.m_maxSize;
    }
    return *this;
}

RoomData& RoomData::operator=(RoomData&& other)
{
    if (this != &other)
    {
        m_creatorId = std::move(other.m_creatorId);
        m_name = std::move(other.m_name);
        m_roomId = std::move(other.m_roomId);
        m_context = std::move(other.m_context);
        m_maxSize = std::move(other.m_maxSize);
    }
    return *this;
}

} // namespace nexilis
