#ifndef CHAT_CLIENT_CHAT_HH
#define CHAT_CLIENT_CHAT_HH

#include <string>
#include <cstdint>
#include <vector>

class Chat
{
public:
    class Room
    {
    public:
        Room(const std::string& name, int maxSize, uint64_t id);

        std::string getName() { return m_name; }
        int getMaxSize() { return m_maxSize; }
        uint64_t getId() { return m_id; }

    private:
        std::string m_name;
        int m_maxSize;
        uint64_t m_id;
    };

    void addRoom(Room&& room);

    int getRoomAmount()
    {
        return static_cast<int>(m_rooms.size());
    }
private:
    std::vector<Room> m_rooms;
};

#endif
