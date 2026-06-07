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
        default:
            Log::error("CommunicationTypeToString no type found!");
            return "";
    }
}

} // namespace nexilis
