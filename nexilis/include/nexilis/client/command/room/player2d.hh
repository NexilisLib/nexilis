#ifndef NEXILIS_CLIENT_COMMAND_ROOM_PLAYER2D_HH
#define NEXILIS_CLIENT_COMMAND_ROOM_PLAYER2D_HH

#include <nexilis/client/base_api_command.hh>
#include <nexilis/client/client_api.hh>

namespace nexilis::client
{

class RoomPlayer2DCommand : public BaseAPICommand
{
public:
    RoomPlayer2DCommand(std::string action, uint64_t client_id, float x, float y)
        : m_action(action), m_client_id(client_id), m_x(x), m_y(y)
    {
    }

    ReadResult execute(ClientAPI& api, ClientAPI::ClientAPIData& data) override
    {
        auto& mtx = data.getRoomsMutex();
        std::lock_guard<std::mutex> lock(*mtx);

        auto& rooms = data.getCurrentlyActiveRooms();

        if (m_action == "position" || m_action == "movement")
        {
            for (auto&& room = rooms.begin(); room != rooms.end(); room++)
            {
                for (auto& client : room->getClients())
                {
                    if (client.getId() == m_client_id)
                    {
                        if (api.isOverlappingAllowed2D())
                        {
                            client.getObject2D().setPosition({m_x, m_y});
                            return ReadResult::success;
                        }
                        else
                        {
                            for (auto& otherClient : room->getClients())
                            {
                                if (otherClient.getId() != m_client_id)
                                {
                                    auto dimensions = client.getObject2D().getDimensions();
                                    auto otherPosition = otherClient.getObject2D().getPosition();
                                    auto otherdimensions = otherClient.getObject2D().getDimensions();

                                    if (
                                            m_x - dimensions.x / 2 < otherPosition.x + otherdimensions.x / 2 &&
                                            m_x + dimensions.x / 2 > otherPosition.x - otherdimensions.x / 2 &&
                                            m_y - dimensions.y / 2 < otherPosition.y + otherdimensions.y / 2 &&
                                            m_y + dimensions.y / 2 > otherPosition.y - otherdimensions.y / 2)
                                    {
                                        return ReadResult::failure;
                                    }
                                }
                            }
                            client.getObject2D().setPosition({m_x, m_y});
                            return ReadResult::success;
                        }
                    }
                }
            }
            return ReadResult::command_execution;
        }
        else if (m_action == "dimension")
        {
            for (auto&& room = rooms.begin(); room != rooms.end(); room++)
            {
                for (auto& client : room->getClients())
                {
                    if (client.getId() == m_client_id)
                    {
                        client.getObject2D().setDimensions({m_x, m_y});
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
};

} // namespace nexilis::client

#endif
