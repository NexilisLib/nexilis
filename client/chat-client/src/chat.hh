#ifndef CHAT_CLIENT_CHAT_HH
#define CHAT_CLIENT_CHAT_HH

#include "debug.hh"
#include "menu.hh"
#include "program_state.hh"
#include "window.hh"

#include <nexilis/client/client_api.hh>

class Chat
{
public:
    /// Constructor.
    Chat(nexilis::client::ClientAPI* clientApi, const std::function<void(const std::vector<uint8_t>&)>& sendTCP, const std::function<void(const std::vector<uint8_t>&, const std::function<void()>&)>& sendTCPWithCallback);

    void update(Window& window, int& hightlight, State& state);

    int getRoomAmount()
    {
        return static_cast<int>(m_rooms->size());
    }

    uint64_t getRoomIdByPosition(int position)
    {
        if (position > static_cast<int>(m_rooms->size()) || m_rooms->empty())
        {
            debug("Cannot select current room");
            return 0;
        }
        return m_rooms->at(static_cast<uint64_t>(position)).getId();
    }

    std::string roomData(const nexilis::client::ClientAPI::Room& room);

private:
    void showRooms(Window& window, int& hightlight);
    void updateChat(Window& window, State& programState);

private:
    nexilis::client::ClientAPI* m_clientApi;
    std::function<void(const std::vector<uint8_t>&)> m_sendTCP;
    std::function<void(const std::vector<uint8_t>&, const std::function<void()>&)> m_sendTCPWithCallback;

    std::vector<nexilis::client::ClientAPI::Room>* m_rooms = nullptr;
};

#endif
