#ifndef NEXILIS_CLIENT_COMMAND_ROOM_PLAYER3D_HH
#define NEXILIS_CLIENT_COMMAND_ROOM_PLAYER3D_HH

#include <nexilis/client/base_api_command.hh>
#include <nexilis/client/client_api.hh>

namespace nexilis::client
{

class RoomPlayer3DCommand : public BaseAPICommand
{
public:
    RoomPlayer3DCommand(std::string action, uint64_t client_id, float x, float y, float z,
                        uint64_t target_id = 0, float damage = 0.0f, float new_health = 0.0f)
        : m_action(action), m_client_id(client_id), m_x(x), m_y(y), m_z(z),
          m_target_id(target_id), m_damage(damage), m_new_health(new_health)
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
        else if (m_action == "damage")
        {
            ClientAPI::DamageEvent event;
            event.target_id = m_target_id;
            event.damage = m_damage;
            event.new_health = m_new_health;
            data.pushDamageEvent(std::move(event));
            return ReadResult::success;
        }
        return ReadResult::not_found;
    }

private:
    std::string m_action;
    uint64_t m_client_id;
    float m_x;
    float m_y;
    float m_z;
    uint64_t m_target_id;
    float m_damage;
    float m_new_health;
};

} // namespace nexilis::client

#endif
