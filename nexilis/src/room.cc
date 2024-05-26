#include <nexilis/common/util.hh>
#include <nexilis/room.hh>
#include <nexilis/log.hh>
#include <sys/types.h>

namespace nexilis
{

/// Room::Client
Room::Client::Client(uint64_t clientId, const std::string& name)
    : m_id(clientId),
      m_name(name)
{
}

Room::Client::Client(const Client& other)
    : m_id(other.m_id),
      m_name(other.m_name)
{
}

Room::Client& Room::Client::operator=(const Client& other)
{
    if (this != &other)
    {
        m_id = other.m_id;
        m_name = other.m_name;
    }
    return *this;
}

Room::Client::Client(Client&& other)
    : m_id(std::move(other.m_id)),
      m_name(std::move(other.m_name))
{
}

Room::Client& Room::Client::operator=(Client&& other)
{
    if (this != &other)
    {
        m_id = std::move(other.m_id);
        m_name = std::move(other.m_name);
    }
    return *this;
}

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
      m_clients(std::move(other.m_clients))
{
}

Room& Room::operator=(Room&& other)
{
    if (this != &other)
    {
        m_data = std::move(other.m_data);
        m_clients = std::move(other.m_clients);
    }
    return *this;
}

bool Room::contains(uint64_t userId)
{
    for (auto c = m_clients.begin(); c != m_clients.end(); c++)
    {
        if (c->getId() == userId)
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
        m_clients.emplace_back(Room::Client(userId));
        Log::info("New user in room: ", getId(), " user: ", userId);
    }
}

void Room::leaveRoom(uint64_t userId)
{
    m_clients.erase(std::remove_if(m_clients.begin(), m_clients.end(),
        [userId](const Client& client) { return client.getId() == userId; }), m_clients.end());
}

} // namespace nexilis
