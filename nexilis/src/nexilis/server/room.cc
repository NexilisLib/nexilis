#include <nexilis/logger/log.hh>
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
      m_clientIds(std::move(other.m_clientIds))
{
}

Room& Room::operator=(Room&& other)
{
    if (this != &other)
    {
        m_clientIds = std::move(other.m_clientIds);
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

} // namespace nexilis::server
