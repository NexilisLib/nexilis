#ifndef NEXILIX_COMMAND_TYPE_HH
#define NEXILIX_COMMAND_TYPE_HH

#include <cstdint>

namespace nexilis
{

enum class MainCommand : uint8_t
{
    protocol_setup = 0x10,
    /*
    UDP 0x10,
    Websocket 0x20
    */

    ping = 0x20,

    // Get information from the server
    info = 0x30,
    /*
    general_info = 0x10
    help
    */

    // Server management
    server_management = 0x40,
    /*
    Start
    Stop
    Restart
    */

    player_management = 0x50,
    /*
    kick = 0x10,
    ban = 0x20,
    unban = 0x30,
    mute = 0x40,
    unmute = 0x50,
    */

    chat = 0x60,
    /*
    say = 0x10,
    whisper = 0x20
    */

    // Get stuff
    give = 0x70,

    // Commands to be overloaded
    update = 0x80
};

}

#endif
