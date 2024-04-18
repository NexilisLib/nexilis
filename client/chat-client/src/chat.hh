#ifndef CHAT_CLIENT_CHAT_HH
#define CHAT_CLIENT_CHAT_HH

/// Forward declare ncurses window.
struct _win_st;

#include "menu.hh"

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

    Chat(Menu::State* state) :
        m_state(state)
    {
    }

    void addRoom(Room&& room);

    void update(_win_st* window, int& hightlight);

    int getRoomAmount()
    {
        return static_cast<int>(m_rooms.size());
    }
private:
    void showRooms(_win_st* window, int& hightlight);

private:
    std::vector<Room> m_rooms;

    Menu::State* m_state = nullptr;
};

#endif
