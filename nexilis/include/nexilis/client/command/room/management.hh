#ifndef NEXILIS_CLIENT_COMMAND_ROOM_MANAGEMENT_HH
#define NEXILIS_CLIENT_COMMAND_ROOM_MANAGEMENT_HH

#include <nexilis/client/base_api_command.hh>
#include <nexilis/client/client_api.hh>

namespace nexilis::client
{

class RoomManagementCommand : public BaseAPICommand
{
public:
    RoomManagementCommand(std::string action, uint64_t room_id, uint64_t client_id)
        : m_action(action), m_room_id(room_id), m_client_id(client_id)
    {
    }

    ReadResult execute(ClientAPI& api, ClientAPI::ClientAPIData& data) override
    {
        auto& mtx = data.getRoomsMutex();
        std::lock_guard<std::mutex> lock(*mtx);

        auto& rooms = data.getCurrentlyActiveRooms();

        if (m_action == "join")
        {
            for (auto& room : rooms)
            {
                if (room.getId() == m_room_id)
                {
                    ClientSession session(m_client_id, &api);
                    room.addClient(std::move(session));
                    return ReadResult::success;
                }
            }
            return ReadResult::error;
        }
        else if (m_action == "leave")
        {
            for (auto& room : rooms)
            {
                if (room.getId() == m_room_id)
                {
                    room.removeClient(m_client_id);
                    return ReadResult::success;
                }
            }
            return ReadResult::error;
        }
        else if (m_action == "create")
        {
            // This would require additional parameters from the JSON that aren't in the current structure
            return ReadResult::not_implemented;
        }
        return ReadResult::error;
    }

private:
    std::string m_action;
    uint64_t m_room_id;
    uint64_t m_client_id;
};

} // namespace nexilis::client

#endif
