#ifndef NEXILIX_COMMAND_TYPE_HH
#define NEXILIX_COMMAND_TYPE_HH

#include <cstdint>

namespace nexilis
{

enum class CommandType : uint8_t
{
    setting = 0,
    getting = 1,
    /**
     *  0. Join room; uint64_t roomId
     *  1. Leave room; void
     *  3. Create room; string roomName
     *
     */
    room = 2,
    authentication = 3,
    server_management = 4,
    player_management = 5,
    communicate = 6,
    error = 7,
    info = 8,
    position = 9,
    dimensions = 10
};

}

#endif
