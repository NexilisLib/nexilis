#ifndef NEXILIX_COMMAND_TYPE_HH
#define NEXILIX_COMMAND_TYPE_HH

#include <cstdint>

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
     *  2:1      Player2D
     *  2:1:0    Set position; Vec2f position
     *  2:1:1    Set dimensions; Vec2f dimensions
     *  2:1:2    2D movement vector; Vec2f movement
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

} // namespace nexilis

#endif
