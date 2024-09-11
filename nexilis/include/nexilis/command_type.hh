#ifndef NEXILIX_COMMAND_TYPE_HH
#define NEXILIX_COMMAND_TYPE_HH

#include <cstdint>
#include <string>

namespace nexilis
{

enum class CommandType : uint8_t
{
    setting = 0,
    getting = 1,

    room = 2,
    /**
         *  2:0      Management
         *  2:0:0    Join room; uint64_t roomId
         *  2:0:1    Leave room; void
         *  2:0:2    Create room; string roomName
         *
         *  2:1      Object2D
         *  2:1:0    Set position; Vec2f position
         *  2:1:1    Set dimensions; Vec2f dimensions
         *
         *  2:2    Communication.
         *  2:2:0  broadcast, send to all; string
    */

    authentication = 3,
    server_management = 4,
    player_management = 5,
    error = 7,
    info = 8,
};

enum class RoomType : uint8_t
{
    management = 0,
    object2D = 1,
    communication = 2
};

enum class ManagementOptions : uint8_t
{
    join = 0,
    leave = 1,
    create = 2
};

enum class Object2DOptions : uint8_t
{
    position = 0,
    dimensions = 1
};

enum class CommunicationOptions : uint8_t
{
    broadcast
};

std::string RoomTypeToString(RoomType type);
std::string ManagementTypeToString(ManagementOptions management);
std::string Object2DTypeToString(Object2DOptions object2D);

}

#endif
