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

#ifndef NEXILIS_SERVER_COMMAND_RESULT_HH
#define NEXILIS_SERVER_COMMAND_RESULT_HH

#include <string>

namespace nexilis::server
{

enum class CommandResult
{
    /// Unimplemented actions.
    unimplemented,

    // Command success.
    success,

    // Logical failure in the command, failing is ok.
    failure,

    // Command is not found.
    not_found,

    // The input for command is not correct.
    invalid_input,

    // There is an error implementing command.
    error,

    // The command usage is unauthorized.
    unauthorized,

    // Command failed in sending the room data command
    failed_room_send
};

std::string asString(CommandResult res);
bool checkResult(CommandResult res);

} // namespace nexilis::server

#endif
