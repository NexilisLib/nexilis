#include <nexilis/common/util.hh>
#include <nexilis/room.hh>
#include <nexilis/log.hh>
#include <sys/types.h>

namespace nexilis
{

Room::Settings::Settings(uint64_t creatorId, const std::string& name, uint32_t maxSize)
    : m_creatorId(creatorId),
      m_name(name),
      m_maxSize(maxSize)
{
}

Room::Settings::Settings(const Settings& other)
    : m_creatorId(other.m_creatorId),
      m_name(other.m_name),
      m_maxSize(other.m_maxSize),
      m_roomId(other.m_roomId)
{
}

Room::Settings::Settings(Settings&& other)
    : m_creatorId(std::move(other.m_creatorId)),
      m_name(std::move(other.m_name)),
      m_maxSize(std::move(other.m_maxSize)),
      m_roomId(std::move(other.m_roomId))
{
}

Room::Settings& Room::Settings::operator=(const Settings& other)
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

Room::Settings& Room::Settings::operator=(Settings&& other)
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

Room::Room(const Settings& settings)
    : m_settings(settings)
{
}

Room::Room(Room&& other)
    : m_settings(std::move(other.m_settings)),
      m_clients(std::move(other.m_clients))
{
}

Room& Room::operator=(Room&& other)
{
    if (this != &other)
    {
        m_settings = std::move(other.m_settings);
        m_clients = std::move(other.m_clients);
    }
    return *this;
}

bool Room::contains(uint64_t userId)
{
    return std::find(m_clients.begin(), m_clients.end(), userId) != m_clients.end();
}

void Room::joinRoom(uint64_t userId)
{
    if (contains(userId))
    {
        Log::warning("User already in this room!");
    }
    else
    {
        m_clients.emplace_back(userId);
        Log::info("New user in room: ", getId(), " user: ", userId);
    }
}

void Room::leaveRoom(uint64_t userId)
{
    auto it = std::find(m_clients.begin(), m_clients.end(), userId);
    if (it != m_clients.end())
    {
        m_clients.erase(it);
    }
    else
    {
        Log::warning("Trying to remove non-existing user from the room");
    }
}

} // namespace nexilis
