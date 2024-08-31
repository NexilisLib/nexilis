#ifndef NEXILIX_COMMAND_TYPE_HH
#define NEXILIX_COMMAND_TYPE_HH

#include <cstdint>

namespace nexilis
{

enum class CommandType : uint8_t
{
    setting = 0,
    getting = 1,
    info = 2,
    authentication = 3,
    server_management = 4,
    player_management = 5,
    communicate = 6,
    error = 7,
    room = 8,
    position = 9,
    dimensions = 10
};

}

#endif
