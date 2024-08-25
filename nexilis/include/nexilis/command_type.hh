#ifndef NEXILIX_COMMAND_TYPE_HH
#define NEXILIX_COMMAND_TYPE_HH

#include <cstdint>

namespace nexilis
{

enum class MainCommand : uint8_t
{
    setting = 0,

    getting = 1,

    // Get information from the server
    info = 2,
    authentication = 3,

    // Server management
    server_management = 4,
    /*
    Start
    Stop
    Restart
    */

    player_management = 5,
    /*
    kick = 0x10,
    ban = 0x20,
    unban = 0x30,
    mute = 0x40,
    unmute = 0x50,
    */

    communicate = 6,
    /*
    say = 0x10,
    whisper = 0x20
    */

    error = 7,

    room = 8,

    position = 9
};

}

#endif
