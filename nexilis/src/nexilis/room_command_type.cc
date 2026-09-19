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
#include <nexilis/room_command_type.hh>

namespace nexilis
{

std::string RoomCommandType::RoomTypeToString(RoomCommandType::Root type)
{
    switch (type)
    {
        case RoomCommandType::Root::management:
            return "management";
        case RoomCommandType::Root::communication:
            return "communication";
        case RoomCommandType::Root::player_2D:
            return "player_2D";
        case RoomCommandType::Root::object_2D:
            return "object_2D";
        case RoomCommandType::Root::player_3D:
            return "player_3D";
        case RoomCommandType::Root::object_3D:
            return "object_3D";
        case RoomCommandType::Root::game_item:
            return "game_item";
        default:
            Log::error("RoomTypeToString no type found!");
            return "undefined";
    }
}

std::string RoomCommandType::ManagementTypeToString(RoomCommandType::Management management)
{
    switch (management)
    {
        case RoomCommandType::Management::join:
            return "join";
        case RoomCommandType::Management::leave:
            return "leave";
        case RoomCommandType::Management::create:
            return "create";
        case RoomCommandType::Management::remove:
            return "remove";
        case RoomCommandType::Management::set_overlap:
            return "set_overlap";
        default:
            Log::error("ManagementTypeToString no type found!");
            return "";
    }
}

std::string RoomCommandType::PlayerTypeToString(RoomCommandType::PlayerType player2D)
{
    switch (player2D)
    {
        case RoomCommandType::PlayerType::position:
            return "position";
        case RoomCommandType::PlayerType::dimension:
            return "dimension";
        case RoomCommandType::PlayerType::movement:
            return "movement";
        case RoomCommandType::PlayerType::shoot:
            return "shoot";
        case RoomCommandType::PlayerType::respawn:
            return "respawn";
        case RoomCommandType::PlayerType::set_team:
            return "set_team";
        case RoomCommandType::PlayerType::leaderboard:
            return "leaderboard";
        case RoomCommandType::PlayerType::audio_event:
            return "audio_event";
        default:
            Log::error("Player2DTypeToString no type found!");
            return "";
    }
}

std::string RoomCommandType::ObjectTypeToString(RoomCommandType::ObjectType object)
{
    switch (object)
    {
        case RoomCommandType::ObjectType::create:
            return "create";
        case RoomCommandType::ObjectType::destroy:
            return "destroy";
        case RoomCommandType::ObjectType::move:
            return "move";
        case RoomCommandType::ObjectType::create_moving:
            return "create_moving";
        default:
            Log::error("Object2DTypeToString no type found!");
            return "";
    }
}

std::string RoomCommandType::GameItemActionToString(RoomCommandType::GameItemAction action)
{
    switch (action)
    {
        case RoomCommandType::GameItemAction::create:
            return "create";
        case RoomCommandType::GameItemAction::update:
            return "update";
        case RoomCommandType::GameItemAction::destroy:
            return "destroy";
        default:
            Log::error("GameItemActionToString no type found!");
            return "";
    }
}

std::string RoomCommandType::CommunicationTypeToString(RoomCommandType::Communication communication)
{
    switch (communication)
    {
        case RoomCommandType::Communication::broadcast:
            return "broadcast";
        case RoomCommandType::Communication::othercast:
            return "othercast";
        case RoomCommandType::Communication::unicast:
            return "unicast";
        case RoomCommandType::Communication::broadcast_encrypted:
            return "broadcast_encrypted";
        default:
            Log::error("CommunicationTypeToString no type found!");
            return "";
    }
}

} // namespace nexilis
