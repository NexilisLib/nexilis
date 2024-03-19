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
    m_rooms.emplace_back(std::move(room));
    Log::info("New room, total amount = ", m_rooms.size());
}

bool RoomStorage::contains(size_t id)
{
    return std::find_if(m_rooms.begin(), m_rooms.end(),
                        [id](const Room& room)
                        {
                            return room.getId() == id;
                        }) != m_rooms.end();
}

Room* RoomStorage::getRoomById(size_t id)
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
