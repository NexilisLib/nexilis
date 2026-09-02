#include <nexilis/logger/log.hh>
#include <nexilis/server/client_storage.hh>
#include <nexilis/server/room.hh>
#include <nexilis/util.hh>

#include <sys/types.h>

namespace nexilis::server
{

Room::Room(const RoomData& data)
    : BaseRoom(data)
{
}

Room::Room(Room&& other)
    : BaseRoom(std::move(other)),
      m_clientIds(std::move(other.m_clientIds)),
      m_broadcasts(std::move(other.m_broadcasts)),
      m_playerHealth(std::move(other.m_playerHealth)),
      m_defaultHealth(other.m_defaultHealth),
      m_deathHandler(std::move(other.m_deathHandler)),
      m_overlappingAllowed(other.m_overlappingAllowed.load())
{
}

Room& Room::operator=(Room&& other)
{
    if (this != &other)
    {
        m_clientIds = std::move(other.m_clientIds);
        m_broadcasts = std::move(other.m_broadcasts);
        m_playerHealth = std::move(other.m_playerHealth);
        m_defaultHealth = other.m_defaultHealth;
        m_deathHandler = std::move(other.m_deathHandler);
        m_overlappingAllowed.store(other.m_overlappingAllowed.load());
        BaseRoom::operator=(std::move(other));
    }
    return *this;
}

bool Room::contains(uint64_t userId)
{
    // Use std::any_of algorithm to check if the user is in the room.
    return std::any_of(m_clientIds.begin(), m_clientIds.end(),
                       [&userId](uint64_t id)
                       { return userId == id; });
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

void Room::addBroadcast(uint64_t clientId, const std::string& message)
{
    m_broadcasts.emplace_back(Broadcast{clientId, message});
}

float Room::getPlayerHealth(uint64_t clientId) const
{
    auto it = m_playerHealth.find(clientId);
    if (it != m_playerHealth.end())
        return it->second;
    return m_defaultHealth;
}

void Room::damagePlayer(uint64_t clientId, float damage)
{
    auto it = m_playerHealth.find(clientId);
    if (it != m_playerHealth.end())
    {
        it->second = std::max(0.0f, it->second - damage);
    }
    else
    {
        m_playerHealth[clientId] = m_defaultHealth - damage;
    }
}

void Room::resetPlayerHealth(uint64_t clientId)
{
    m_playerHealth[clientId] = m_defaultHealth;
}

void Room::setDeathHandler(DeathHandler handler)
{
    m_deathHandler = std::move(handler);
}

void Room::onPlayerDied(uint64_t killerId, uint64_t victimId)
{
    if (m_deathHandler)
    {
        m_deathHandler(*this, killerId, victimId);
    }
}

bool Room::broadcastToAll(const nx_data& data)
{
    bool all_success = true;
    for (auto clientId : m_clientIds)
    {
        auto* client = ClientStorage::getClientById(clientId);
        if (!client)
            continue;

        if (client->isBoostTCPSet())
        {
            if (!client->boostTCPSend(data))
                all_success = false;
        }
        else if (client->isBoostUDPSet())
        {
            if (!client->boostUDPSend(data))
                all_success = false;
        }
        else if (client->isUnixStreamSet())
        {
            if (!client->unixStreamSend(data))
                all_success = false;
        }
        else
        {
            all_success = false;
        }
    }
    return all_success;
}

} // namespace nexilis::server
