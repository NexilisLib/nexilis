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

CommandResult ServerImpl::room_gameitem_update(const DefaultArgs& args)
{
    auto& user = args.getUser();
    auto data = args.getData();
    auto payload = Util::removeAmountOfBytesFromVector(data, 3);

    auto itemId = Util::uint64FromFront(payload);
    payload = Util::removeAmountOfBytesFromVector(payload, 8);
    auto status = Util::convertToString(payload);

    auto room = RoomStorage::getRoomById(user.getRoomId());
    if (!room)
    {
        Log::error("Client room not found!");
        return CommandResult::failure;
    }

    room->updateGameItemStatus(itemId, status);

    std::map<std::string, boost::json::value> params{
            {"id", boost::json::value(itemId)},
            {"status", boost::json::value(status)}};

    auto roomCommand = Command::createRoomCommand(user.getRoomId(), user, data, params, args.getMessageId());
    if (Command::sendRoomCommand(roomCommand, user, args.getProtocol()))
        return CommandResult::success;
    else
        return CommandResult::failed_room_send;
}

} // namespace nexilis::server
