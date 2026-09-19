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

#ifndef NEXILIS_CLIENT_COMMAND_PARSER_HH
#define NEXILIS_CLIENT_COMMAND_PARSER_HH

#include <nexilis/client/base_api_command.hh>
#include <nexilis/client/command/error.hh>
#include <nexilis/command_type.hh>

namespace nexilis::client
{

class CommandParser
{
public:
    static std::unique_ptr<BaseAPICommand> parse(const boost::json::object& json);

private:
    static std::unique_ptr<BaseAPICommand> parseSettingCommand(const boost::json::object& json, std::string_view type);
    static std::unique_ptr<BaseAPICommand> parseGettingCommand(const boost::json::object& json, std::string_view type);
    static std::unique_ptr<BaseAPICommand> parseRoomCommand(const boost::json::object& json, std::string_view type);
    static std::unique_ptr<BaseAPICommand> parseErrorCommand(std::string_view type);
};

} // namespace nexilis::client

#endif
