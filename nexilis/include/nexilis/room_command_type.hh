#ifndef NEXILIS_ROOM_COMMAND_TYPE_HH
#define NEXILIS_ROOM_COMMAND_TYPE_HH

#include <cstdint>
#include <string>

namespace nexilis
{

class RoomCommandType
{
public:
    enum class Root : uint8_t
    {
        management = 0,
        player2D = 1,
        object2D = 2,
        communication = 3
    };

    enum class Player2D : uint8_t
    {
        position = 0,
        dimensions = 1,
        movement = 2
    };

    enum class Management : uint8_t
    {
        join = 0,
        leave = 1,
        create = 2
    };

    enum class Object2D : uint8_t
    {
        create = 0,
        destroy = 1,
        move = 2,
        createMoving = 3
    };

    enum class Communication : uint8_t
    {
        broadcast
    };

    static std::string RoomTypeToString(Root type);
    static std::string Player2DTypeToString(Player2D player2D);
    static std::string Object2DTypeToString(Object2D object2D);
    static std::string ManagementTypeToString(Management management);
};

}

#endif