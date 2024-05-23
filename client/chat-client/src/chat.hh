#ifndef CHAT_CLIENT_CHAT_HH
#define CHAT_CLIENT_CHAT_HH

#include "menu.hh"
#include "window.hh"
#include "debug.hh"
#include "program_state.hh"

#include <nexilis/client_api.hh>

class Chat
{
public:
    /// Constructor.
    Chat(nexilis::ClientAPI* clientApi, const std::function<void(const std::vector<uint8_t>&)>& sendTCP);

    void update(Window& window, int& hightlight, State& state);

    int getRoomAmount()
    {
        return static_cast<int>(m_rooms.size());
    }

    uint64_t getRoomIdByPosition(int position)
    {
        if (position > static_cast<int>(m_rooms.size()) || m_rooms.empty())
        {
            debug("Cannot select current room");
            return 0;
        }
        return m_rooms[static_cast<uint64_t>(position)].getRoomId();
    }

    std::string roomData(const nexilis::ClientAPI::Room& room);

private:
    void showRooms(Window& window, int& hightlight, State& state);
    void updateChat(Window& window, State& programState);

private:
    nexilis::ClientAPI* m_clientApi;
    std::function<void(const std::vector<uint8_t>&)> m_sendTCP;

    std::vector<nexilis::ClientAPI::Room> m_rooms;
};

#endif
