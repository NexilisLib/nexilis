#ifndef NEXILIX_COMMAND_TYPE_HH
#define NEXILIX_COMMAND_TYPE_HH

#include <cstdint>

namespace nexilis
{

enum class MainCommand : uint8_t
{
    set = 0x10,

    get = 0x20,
    /*
    UDP 0x10,
    Websocket 0x20
    */

    ping = 0x30,

    // Get information from the server
    info = 0x40,
    /*
    general_info = 0x10
    help
    */
    authentication = 0x50,

    // Server management
    server_management = 0x60,
    /*
    Start
    Stop
    Restart
    */

    player_management = 0x70,
    /*
    kick = 0x10,
    ban = 0x20,
    unban = 0x30,
    mute = 0x40,
    unmute = 0x50,
    */

    communicate = 0x80,
    /*
    say = 0x10,
    whisper = 0x20
    */

    update = 0x90,

    error = 0xa
};

}

#endif
