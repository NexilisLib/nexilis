#ifndef NEXILIS_CLIENT_COMMAND_ROOM_PLAYER3D_HH
#define NEXILIS_CLIENT_COMMAND_ROOM_PLAYER3D_HH

#include <nexilis/client/base_api_command.hh>
#include <nexilis/client/client_api.hh>

namespace nexilis::client
{

class RoomPlayer3DCommand : public BaseAPICommand
{
public:
    RoomPlayer3DCommand(std::string action, uint64_t client_id, float x, float y, float z)
        : m_action(action), m_client_id(client_id), m_x(x), m_y(y), m_z(z)
    {
    }

    ReadResult execute(ClientAPI&, ClientAPI::ClientAPIData& data) override
    {
        auto& mtx = data.getRoomsMutex();
        std::lock_guard<std::mutex> lock(*mtx);

        auto& rooms = data.getCurrentlyActiveRooms();

        if (m_action == "position")
        {
            for (auto&& room = rooms.begin(); room != rooms.end(); room++)
            {
                for (auto& client : room->getClients())
                {
                    if (client.getId() == m_client_id)
                    {
                        client.getObject3D().setPosition(Vector3(m_x, m_y, m_z));
                        return ReadResult::success;
                    }
                }
            }
            return ReadResult::command_execution;
        }
        else if (m_action == "dimensions")
        {
            for (auto&& room = rooms.begin(); room != rooms.end(); room++)
            {
                for (auto& client : room->getClients())
                {
                    if (client.getId() == m_client_id)
                    {
                        client.getObject3D().setDimensions(Vector3(m_x, m_y, m_z));
                        return ReadResult::success;
                    }
                }
            }
            return ReadResult::command_execution;
        }
        else if (m_action == "movement")
        {
            for (auto&& room = rooms.begin(); room != rooms.end(); room++)
            {
                for (auto& client : room->getClients())
                {
                    if (client.getId() == m_client_id)
                    {
                        client.getObject3D().setPosition(Vector3(m_x, m_y, m_z));
                        return ReadResult::success;
                    }
                }
            }
            return ReadResult::command_execution;
        }
        return ReadResult::not_found;
    }

private:
    std::string m_action;
    uint64_t m_client_id;
    float m_x;
    float m_y;
    float m_z;
};

} // namespace nexilis::client

#endif
