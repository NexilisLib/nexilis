#include <nexilis/common/util.hh>
#include <nexilis/log.hh>
#include <nexilis/room.hh>
#include <sys/types.h>

namespace nexilis
{

/// Room::Data
Room::Data::Data(uint64_t creatorId, const std::string& name, uint32_t maxSize)
    : m_creatorId(creatorId),
      m_name(name),
      m_maxSize(maxSize)
{
}

Room::Data::Data(const Data& other)
    : m_creatorId(other.m_creatorId),
      m_name(other.m_name),
      m_maxSize(other.m_maxSize),
      m_roomId(other.m_roomId)
{
}

Room::Data::Data(Data&& other)
    : m_creatorId(std::move(other.m_creatorId)),
      m_name(std::move(other.m_name)),
      m_maxSize(std::move(other.m_maxSize)),
      m_roomId(std::move(other.m_roomId))
{
}

Room::Data& Room::Data::operator=(const Data& other)
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

Room::Data& Room::Data::operator=(Data&& other)
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

Room::Room(const Data& data)
    : m_data(data)
{
}

Room::Room(Room&& other)
    : m_data(std::move(other.m_data)),
      m_clientIds(std::move(other.m_clientIds))
{
}

Room& Room::operator=(Room&& other)
{
    if (this != &other)
    {
        m_data = std::move(other.m_data);
        m_clientIds = std::move(other.m_clientIds);
    }
    return *this;
}

bool Room::contains(uint64_t userId)
{
    for (auto& client : m_clientIds)
    {
        if (client == userId)
        {
            return true;
        }
    }
    return false;
}

void Room::joinRoom(uint64_t userId)
{
    if (contains(userId))
    {
        Log::warning("User already in this room!");
    }
    else
    {
        m_clientIds.emplace_back(userId);
        Log::info("New user in room: ", getId(), " user: ", userId);
    }
}

void Room::leaveRoom(uint64_t userId)
{
    m_clientIds.erase(std::remove_if(m_clientIds.begin(), m_clientIds.end(),
                                     [&userId](uint64_t id)
                                     { return userId == id; }),
                      m_clientIds.end());
}

} // namespace nexilis
