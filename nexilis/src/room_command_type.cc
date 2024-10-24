#include <nexilis/room_command_type.hh>
#include <nexilis/logger/log.hh>

namespace nexilis
{

std::string RoomCommandType::RoomTypeToString(RoomCommandType::Root type)
{
    switch (type)
    {
        case RoomCommandType::Root::management:
            return "management";
        case RoomCommandType::Root::player2D:
            return "player2D";
        case RoomCommandType::Root::object2D:
            return "object2D";
        case RoomCommandType::Root::communication:
            return "communication";
        default:
            Log::error("RoomTypeToString no type found!");
            return "";
    }
}

std::string RoomCommandType::Player2DTypeToString(RoomCommandType::Player2D player2D)
{
    switch (player2D)
    {
        case RoomCommandType::Player2D::position:
            return "position";
        case RoomCommandType::Player2D::dimensions:
            return "dimensions";
        case RoomCommandType::Player2D::movement:
            return "movement";
        default:
            Log::error("Player2DTypeToString no type found!");
            return "";
    }
}

std::string RoomCommandType::Object2DTypeToString(RoomCommandType::Object2D object2D)
{
    switch (object2D)
    {
        case RoomCommandType::Object2D::create:
            return "create";
        case RoomCommandType::Object2D::destroy:
            return "destroy";
        case RoomCommandType::Object2D::move:
            return "move";
        case RoomCommandType::Object2D::createMoving:
            return "createMoving";
        case RoomCommandType::Object2D::createMovingTest:
            return "createMovingTest";
        default:
            Log::error("Object2DTypeToString no type found!");
            return "";
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
        default:
            Log::error("ManagementTypeToString no type found!");
            return "";
    }
}


} // namespace nexilis

