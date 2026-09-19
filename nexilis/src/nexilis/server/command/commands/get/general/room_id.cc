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

namespace nexilis::server
{

CommandResult ServerImpl::get_general_roomId(const DefaultArgs& args)
{
    Log::debug("getting::general::client_id", args);

    std::map<std::string, boost::json::value> params{
            {"client_id", boost::json::value(args.getUser().getId())}};

    auto data = Command::clientMessageData(CommandType::getting, "room_id", args.getMessageId(), params);
    Command::sendMessageToClient(data, args.getUser(), args.getProtocol());

    return CommandResult::success;
}
} // namespace nexilis::server
