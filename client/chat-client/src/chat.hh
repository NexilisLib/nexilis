#ifndef CHAT_CLIENT_CHAT_HH
#define CHAT_CLIENT_CHAT_HH

/// Forward declare ncurses window.
struct _win_st;

#include "menu.hh"
#include "debug.hh"

#include <nexilis/client_api.hh>

#include <sstream>

class Chat
{
public:
    /// Constructor.
    Chat(Menu::State* state, nexilis::ClientAPI* clientApi);

    //void addRoom(Room&& room);

    void update(_win_st* window, int& hightlight);

    int getRoomAmount()
    {
        return static_cast<int>(m_rooms.size());
    }

    uint64_t getRoomIdByPosition(int position)
    {
        std::stringstream ss;
        ss << "Position: " << position;
        debug(ss.str());
        return m_rooms[static_cast<size_t>(position)].getRoomId();
    }

private:
    void showRooms(_win_st* window, int& hightlight);

private:
    Menu::State* m_state = nullptr;
    nexilis::ClientAPI* m_clientApi;

    std::vector<nexilis::ClientAPI::Room> m_rooms;
};

#endif
