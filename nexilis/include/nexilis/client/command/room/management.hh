#ifndef NEXILIS_CLIENT_COMMAND_ROOM_MANAGEMENT_HH
#define NEXILIS_CLIENT_COMMAND_ROOM_MANAGEMENT_HH

#include <nexilis/client/base_api_command.hh>
#include <nexilis/client/client_api.hh>

namespace nexilis::client
{

class RoomManagementCommand : public BaseAPICommand
{
public:
    RoomManagementCommand(std::string action, uint64_t room_id, uint64_t client_id,
                          std::string room_name = "", uint64_t room_context = 0,
                          std::string username = "")
        : m_action(action), m_room_id(room_id), m_client_id(client_id),
          m_room_name(std::move(room_name)), m_room_context(room_context),
          m_username(std::move(username))
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
                    for (auto& client : room.getClients())
                    {
                        if (client.getId() == m_client_id)
                        {
                            if (!m_username.empty())
                            {
                                client.setUsername(m_username);
                            }
                            return ReadResult::success;
                        }
                    }

                    ClientSession session(m_client_id, &api);
                    session.setUsername(m_username);
                    room.addClient(std::move(session));
                    return ReadResult::success;
                }
            }
            return ReadResult::command_execution;
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
            return ReadResult::command_execution;
        }
        else if (m_action == "create")
        {
            auto roomData = RoomData(m_client_id, m_room_name, m_room_id,
                                     static_cast<RoomData::Context>(m_room_context));
            rooms.emplace_back(Room(roomData, std::vector<ClientSession>()));
            return ReadResult::success;
        }
        return ReadResult::not_found;
    }

private:
    std::string m_action;
    uint64_t m_room_id;
    uint64_t m_client_id;
    std::string m_room_name;
    uint64_t m_room_context;
    std::string m_username;
};

} // namespace nexilis::client

#endif
