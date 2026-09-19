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

#include <nexilis/server/command/command.hh>
#include <nexilis/server/command/commands.hh>
#include <nexilis/server/room_storage.hh>

namespace nexilis::server
{

CommandResult ServerImpl::room_object2d_move(const DefaultArgs& args)
{
    auto& user = args.getUser();

    // Get messagedata.
    auto payload = Util::removeAmountOfBytesFromVector(args.getData(), 3);
    auto objectId = Util::uint64FromFront(payload);
    auto position = Util::vector2fFromFront(Util::removeAmountOfBytesFromVector(payload, 8));

    // Get object from server storage.
    auto room = RoomStorage::getRoomById(user.getRoomId());
    if (!room)
    {
        Log::error("Client room not found!");
        return CommandResult::failure;
    }

    auto object = room->getObject2DById(objectId);
    if (!object)
    {
        Log::error("Object not found with id: ", objectId);
        return CommandResult::failure;
    }

    auto oldPosition = object->getPosition();
    auto newPosition = oldPosition + position;

    std::map<std::string, boost::json::value> params{
            {"id", boost::json::value(objectId)},
            {"x", boost::json::value(newPosition.x)},
            {"y", boost::json::value(newPosition.y)}};

    // Move object in server storage.
    object->setPosition(newPosition);

    auto roomCommand = Command::createRoomCommand(user.getRoomId(), user, args.getData(), params, args.getMessageId());
    if (Command::sendRoomCommand(roomCommand, user, args.getProtocol()))
    {
        return CommandResult::success;
    }
    else
    {
        return CommandResult::failed_room_send;
    }
}

} // namespace nexilis::server
