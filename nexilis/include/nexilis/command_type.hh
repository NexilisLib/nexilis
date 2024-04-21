#ifndef NEXILIX_COMMAND_TYPE_HH
#define NEXILIX_COMMAND_TYPE_HH

#include <cstdint>

namespace nexilis
{

enum class MainCommand : uint8_t
{
    set = 0,

    get = 1,
    ping = 2,

    // Get information from the server
    info = 3,
    authentication = 4,

    // Server management
    server_management = 5,
    /*
    Start
    Stop
    Restart
    */

    player_management = 6,
    /*
    kick = 0x10,
    ban = 0x20,
    unban = 0x30,
    mute = 0x40,
    unmute = 0x50,
    */

    communicate = 7,
    /*
    say = 0x10,
    whisper = 0x20
    */

    update = 8,

    error = 9,

    room = 10
};

}

#endif
