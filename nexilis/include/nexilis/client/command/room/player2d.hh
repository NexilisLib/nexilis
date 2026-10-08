/* Copyright (C) 2026 Valtteri Viirret
   This file is part of the Nexilis Project.

   This file is free software: you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as
   published by the Free Software Foundation, either version 3 of the
   License, or (at your option) any later version.

   This file is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with this file.  If not, see <https://gnu.org>. */

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

class RoomPlayer2DShootCommand : public BaseAPICommand
{
public:
    RoomPlayer2DShootCommand(uint64_t client_id, float x, float y)
        : m_client_id(client_id), m_x(x), m_y(y)
    {
    }

    ReadResult execute(ClientAPI& api, ClientAPI::ClientAPIData& data) override
    {
        auto& mtx = data.getRoomsMutex();
        std::lock_guard<std::mutex> lock(*mtx);

        auto& rooms = data.getCurrentlyActiveRooms();

        for (auto&& room = rooms.begin(); room != rooms.end(); room++)
        {
            for (auto& client : room->getClients())
            {
                if (client.getId() == m_client_id)
                {
                    // Notify the client API about the shoot event
                    // The game client will handle creating the visual projectile
                    api.onPlayer2DShoot(m_client_id, {m_x, m_y});
                    return ReadResult::success;
                }
            }
        }
        return ReadResult::command_execution;
    }

private:
    uint64_t m_client_id;
    float m_x;
    float m_y;
};

} // namespace nexilis::client

#endif
