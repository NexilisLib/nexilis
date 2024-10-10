#include <nexilis/command_type.hh>
#include <nexilis/logger/log.hh>

namespace nexilis
{

std::string RoomTypeToString(RoomType type)
{
    switch (type)
    {
        case RoomType::management:
            return "management";
        case RoomType::player2D:
            return "object2D";
        case RoomType::communication:
            return "communication";
        default:
            Log::error("RoomTypeToString no type found!");
            return "";
    }
}

std::string ManagementTypeToString(ManagementOptions management)
{
    switch (management)
    {
        case ManagementOptions::join:
            return "join";
        case ManagementOptions::leave:
            return "leave";
        case ManagementOptions::create:
            return "create";
        default:
            Log::error("ManagementTypeToString no type found!");
            return "";
    }
}
std::string Player2DTypeToString(Player2DOptions player2D)
{
    switch (player2D)
    {
        case Player2DOptions::position:
            return "position";
        case Player2DOptions::dimensions:
            return "dimensions";
        case Player2DOptions::movement:
            return "movement";
        default:
            Log::error("Object2DTypeToString no type found!");
            return "";
    }
}

} // namespace nexilis
