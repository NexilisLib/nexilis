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
#include <nexilis/server/command/command_result.hh>

namespace nexilis::server
{

std::string asString(CommandResult res)
{
    switch (res)
    {
        case CommandResult::error:
            return "error";
        case CommandResult::failure:
            return "failure";
        case CommandResult::invalid_input:
            return "invalid_input";
        case CommandResult::success:
            return "success";
        case CommandResult::not_found:
            return "not_found";
        case CommandResult::unauthorized:
            return "unauthorized";
        case CommandResult::unimplemented:
            return "unimplemented";
        case CommandResult::failed_room_send:
            return "failed_room_send";
    }
    return "";
}

bool checkResult(CommandResult result)
{
    switch (result)
    {
        case CommandResult::success:
            return true;

        case CommandResult::unauthorized:
            Log::error("Unauthorized");
            break;

        case CommandResult::unimplemented:
            Log::error("Unimplemented");
            break;

        case CommandResult::not_found:
            Log::error("Not found");
            break;

        case CommandResult::invalid_input:
            Log::error("Invalid input");
            break;

        case CommandResult::error:
            Log::error("Error");
            break;

        case CommandResult::failure:
            Log::error("Failure");
            break;
        case CommandResult::failed_room_send:
            Log::error("Failed room send");
            break;
    }
    return false;
}

} // namespace nexilis::server
