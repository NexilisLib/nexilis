#include <nexilis/room_storage.hh>
#include <nexilis/log.hh>

namespace nexilis
{

std::vector<Room> RoomStorage::m_rooms = {};

std::vector<Room>& RoomStorage::getAllRooms()
{
    return m_rooms;
}

void RoomStorage::add(Room&& room)
{
    Log::info("New room: ", room.getName(), " id: ", room.getId());
    m_rooms.emplace_back(std::move(room));
    Log::info("Total room amount = ", m_rooms.size());
}

bool RoomStorage::contains(uint64_t id)
{
    return std::find_if(m_rooms.begin(), m_rooms.end(),
                        [id](const Room& room)
                        {
                            return room.getId() == id;
                        }) != m_rooms.end();
}

Room* RoomStorage::getRoomById(uint64_t id)
{
    auto it = std::find_if(m_rooms.begin(), m_rooms.end(),
                           [id](const Room& room)
                           {
                               return room.getId() == id;
                           });

    if (it != m_rooms.end())
    {
        return &(*it);
    }
    else
    {
        return nullptr;
    }
}

} // namespace nexilis
