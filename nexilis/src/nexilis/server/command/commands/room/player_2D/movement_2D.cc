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

#include <nexilis/server/client_storage.hh>
#include <nexilis/server/command/command.hh>
#include <nexilis/server/command/commands.hh>
#include <nexilis/server/movement.hh>
#include <nexilis/server/room_storage.hh>

namespace nexilis::server
{

CommandResult ServerImpl::room_player2d_movement(const DefaultArgs& args)
{
    // Movement 2D, creates a thread that sends the new position with time of delta.
    // In 16 thread CPU: when delta = 0.1f -> ~6 updates.
    auto data = args.getData();
    auto& user = args.getUser();
    auto& protocol = args.getProtocol();
    auto messageId = args.getMessageId();

    auto payload = Util::removeAmountOfBytesFromVector(data, 3);
    auto vecX = Util::floatFromFront(payload);
    auto vecY = Util::floatFromFront(Util::removeAmountOfBytesFromVector(payload, 4));
    auto delta = Util::floatFromFront(Util::removeAmountOfBytesFromVector(payload, 8));
    auto movementVector = Vector2f(vecX, vecY);
    auto mtx = std::make_shared<std::mutex>();

    // Capture start position before the thread so each tick computes
    // startPos + linear(progress, distance) rather than accumulating
    // eased offsets onto an already-moved position (which overshoots).
    Vector2f startPosition = user.getObject2D().getPosition();

    // clang-format off
    std::thread([mtx, movementVector, startPosition, &user, data, &protocol, messageId, delta]()
    {
        try
        {
            Command::runWithTickrate(60.0f, delta, [&mtx, movementVector, startPosition, &user, data, &protocol, messageId](double progress)
            {
                auto* clientRoom = RoomStorage::getRoomById(user.getRoomId());
                assert(clientRoom);

                // Linear gives responsive, predictable movement for a shooter.
                // Add to startPosition (not currentPosition) to avoid per-tick accumulation drift.
                double deltaX = Movement::linear(progress, movementVector.x);
                double deltaY = Movement::linear(progress, movementVector.y);

                Vector2f dimensions = user.getObject2D().getDimensions();
                auto newMovedPosition = Vector2f(startPosition.x + deltaX, startPosition.y + deltaY);

                bool limitedMovement = false;
                for (auto& c : clientRoom->getClients())
                {
                    User* roomClient = ClientStorage::getClientById(c);

                    if (roomClient && roomClient->getId() != user.getId())
                    {
                        Vector2f roomClientPosition;
                        Vector2f roomClientDimensions;
                        {
                            std::lock_guard<std::mutex> lock(*mtx);
                            roomClientPosition = roomClient->getObject2D().getPosition();
                            roomClientDimensions = roomClient->getObject2D().getDimensions();
                        }

                        // Assumed square.
                        if (
                                newMovedPosition.x - dimensions.x / 2 < roomClientPosition.x + roomClientDimensions.x / 2 &&
                                newMovedPosition.x + dimensions.x / 2 > roomClientPosition.x - roomClientDimensions.x / 2 &&
                                newMovedPosition.y - dimensions.y / 2 < roomClientPosition.y + roomClientDimensions.y / 2 &&
                                newMovedPosition.y + dimensions.y / 2 > roomClientPosition.y - roomClientDimensions.y / 2
                           )
                        {
                            Log::info("Players tried to hit each other!");
                            limitedMovement = true;
                            return;
                        }
                    }
                }

                if (!limitedMovement)
                {
                    std::lock_guard<std::mutex> lock(*mtx);
                    user.getObject2D().setPosition(newMovedPosition);
                    std::map<std::string, boost::json::value> params = {
                        {"x", boost::json::value(newMovedPosition.x)},
                        {"y", boost::json::value(newMovedPosition.y)},
                    };
                    auto roomCommand = Command::createRoomCommand(user.getRoomId(), user, data, params, messageId);
                    Command::sendRoomCommand(roomCommand, user, protocol);
                }
            });
        }
        catch (std::exception& e)
        {
            Log::error(e.what());
        }
    })
    .detach();
    return CommandResult::success;
    }
// clang-format on
} // namespace nexilis::server
