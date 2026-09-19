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

#include <nexilis/command_type.hh>

namespace nexilis
{

std::string commandTypeAsString(CommandType command_type)
{
    switch (command_type)
    {
        case CommandType::setting:
            return "setting";
        case CommandType::getting:
            return "getting";
        case CommandType::room:
            return "room";
        case CommandType::authentication:
            return "authentication";
        case CommandType::server_management:
            return "server_management";
        case CommandType::player_management:
            return "player_management";
        case CommandType::error:
            return "error";
        case CommandType::undefined:
            return "undefined";
    }
    return "undefined";
}

CommandType commandTypeFromString(const std::string& str)
{
    if (str == "setting")
        return CommandType::setting;
    else if (str == "getting")
        return CommandType::getting;
    else if (str == "room")
        return CommandType::room;
    else if (str == "authentication")
        return CommandType::authentication;
    else if (str == "server_management")
        return CommandType::server_management;
    else if (str == "player_management")
        return CommandType::player_management;
    else if (str == "error")
        return CommandType::error;
    else
        return CommandType::undefined;
}

} // namespace nexilis
