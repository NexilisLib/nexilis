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

#include <nexilis/logger/log.hh>
#include <nexilis/movement_type.hh>
#include <nexilis/object/object_2d.hh>
#include <nexilis/server/client_storage.hh>
#include <nexilis/server/command/command.hh>
#include <nexilis/server/command/commands.hh>
#include <nexilis/server/movement.hh>
#include <nexilis/server/room_storage.hh>

namespace nexilis::server
{

CommandResult ServerImpl::room_player2d_shoot(const DefaultArgs& args)
{
    auto& user = args.getUser();
    auto data = args.getData();
    auto& protocol = args.getProtocol();
    auto messageId = args.getMessageId();

    // Get messagedata: direction vector (x, y)
    auto payload = Util::removeAmountOfBytesFromVector(data, 3);
    auto dirX = Util::floatFromFront(payload);
    auto dirY = Util::floatFromFront(Util::removeAmountOfBytesFromVector(payload, 4));
    Vector2f direction(dirX, dirY);

    // Normalize direction
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length > 0.0f)
    {
        direction.x /= length;
        direction.y /= length;
    }

    // Get player position and dimensions
    auto startPosition = user.getObject2D().getPosition();
    auto dimensions = user.getObject2D().getDimensions();

    // Calculate projectile starting position (at player edge in shooting direction)
    Vector2f projectileStartPos(
            startPosition.x + direction.x * (dimensions.x / 2.0f + 5.0f),
            startPosition.y + direction.y * (dimensions.y / 2.0f + 5.0f));

    // Create projectile object
    Vector2f projectileDimensions(10.0f, 10.0f);
    auto projectile = nexilis::Object2D(Util::getRandomUint64(), projectileStartPos, projectileDimensions);
    projectile.setFilepath("assets/circle.png"); // Use circle asset for projectile
    uint64_t projectileId = projectile.getId();
    Log::info("Player ", user.getId(), " shot projectile with id: ", projectileId);

    // Add projectile to room
    auto room = RoomStorage::getRoomById(user.getRoomId());
    room->addObject(std::move(projectile));

    // Broadcast projectile creation to all clients in room
    std::map<std::string, boost::json::value> params{
            {"createMovingType", boost::json::value("create")},
            {"x", boost::json::value(projectileStartPos.x)},
            {"y", boost::json::value(projectileStartPos.y)},
            {"width", boost::json::value(projectileDimensions.x)},
            {"height", boost::json::value(projectileDimensions.y)},
            {"filepath", boost::json::value("assets/circle.png")},
            {"id", boost::json::value(projectileId)}};
    auto roomCommand = Command::createRoomCommand(user.getRoomId(), user, data, params, messageId);
    Command::sendRoomCommand(roomCommand, user, protocol);

    // Set up projectile movement
    // Projectile moves at 500 units per second in the shooting direction
    float projectileSpeed = 500.0f;
    Vector2f projectileMovement(direction.x * projectileSpeed, direction.y * projectileSpeed);
    float deltaTime = 0.1f; // Update interval

    std::function<double(double, double)> movementFunction = [](double progress, double totalDistance) -> double
    {
        return Movement::linear(progress, totalDistance);
    };

    auto base_move = std::make_unique<Movement2D>(MovementData(projectileId, deltaTime, data, messageId), projectileMovement, movementFunction);
    Movement::object2D(std::move(base_move), user, protocol).detach();

    return CommandResult::success;
}

} // namespace nexilis::server
