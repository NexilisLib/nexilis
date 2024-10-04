#include <nexilis/command_type.hh>
#include <nexilis/log.hh>

namespace nexilis
{

std::string RoomTypeToString(RoomType type)
{
    switch (type)
    {
        case RoomType::management:
            return "management";
        case RoomType::object2D:
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
std::string Object2DTypeToString(Object2DOptions object2D)
{
    switch (object2D)
    {
        case Object2DOptions::position:
            return "position";
        case Object2DOptions::dimensions:
            return "dimensions";
        case Object2DOptions::movement:
            return "movement";
        default:
            Log::error("Object2DTypeToString no type found!");
            return "";
    }
}

} // namespace nexilis
