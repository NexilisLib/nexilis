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
        case RoomCommandType::Root::player2D:
            return "player2D";
        case RoomCommandType::Root::object2D:
            return "object2D";
        case RoomCommandType::Root::player3D:
            return "player3D";
        case RoomCommandType::Root::object3D:
            return "object3D";
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
        default:
            Log::error("ManagementTypeToString no type found!");
            return "";
    }
}

std::string RoomCommandType::PlayerTypeToString(RoomCommandType::Player2D player2D)
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

std::string RoomCommandType::ObjectTypeToString(RoomCommandType::Object2D object2D)
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

std::string RoomCommandType::PlayerTypeToString(RoomCommandType::Player3D player3D)
{
    switch (player3D)
    {
        case RoomCommandType::Player3D::position:
            return "position";
        case RoomCommandType::Player3D::dimensions:
            return "dimensions";
        case RoomCommandType::Player3D::movement:
            return "movement";
        default:
            Log::error("Player3DTypeToString no type found!");
            return "";
    }
}

std::string RoomCommandType::ObjectTypeToString(RoomCommandType::Object3D object3D)
{
    switch (object3D)
    {
        case RoomCommandType::Object3D::create:
            return "create";
        case RoomCommandType::Object3D::destroy:
            return "destroy";
        case RoomCommandType::Object3D::move:
            return "move";
        default:
            Log::error("Object3DTypeToString no type found!");
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
