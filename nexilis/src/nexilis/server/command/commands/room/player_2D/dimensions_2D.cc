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

CommandResult ServerImpl::room_player2d_dimension(const DefaultArgs& args)
{
    auto& user = args.getUser();
    auto payload = Util::removeAmountOfBytesFromVector(args.getData(), 3);
    auto vector = Util::convertToVector2(payload);
    Log::debug("Dimension x:", vector.x, " y:", vector.y);

    auto currentRoom = RoomStorage::getRoomById(user.getRoomId());
    if (!currentRoom)
    {
        Log::error("Client not currently in room.");
        return CommandResult::failure;
    }

    std::map<std::string, boost::json::value> params{
            {"width", boost::json::value(vector.x)},
            {"height", boost::json::value(vector.y)}};

    user.getObject2D().setDimensions(vector);

    auto roomCommand = Command::createRoomCommand(user.getRoomId(), user, args.getData(), params, args.getMessageId());
    if (Command::sendRoomCommand(roomCommand, user, args.getProtocol()))
    {
        return CommandResult::success;
    }
    return CommandResult::failure;
}

} // namespace nexilis::server
