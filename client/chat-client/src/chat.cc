#include "chat.hh"

Chat::Room::Room(const std::string& name, int maxSize, uint64_t id) :
    m_name(name),
    m_maxSize(maxSize),
    m_id(id)
{
}

void Chat::addRoom(Room&& room)
{
    m_rooms.emplace_back(room);
}
