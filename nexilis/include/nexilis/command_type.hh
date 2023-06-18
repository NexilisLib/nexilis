#ifndef NEXILIX_COMMAND_TYPE_HH
#define NEXILIX_COMMAND_TYPE_HH

namespace nexilis
{

enum class MainCommand : unsigned char
{
    ping = 0x10,

    // Get information from the server
    info = 0x20,
    /*
    general_info = 0x10
    status = 0x20,
    help
    */

    // Server management
    server_management = 0x30,
    /*
    Start
    Stop
    Restart
    */

    player_management = 0x40,
    /*
    kick = 0x10,
    ban = 0x20,
    unban = 0x30,
    mute = 0x40,
    unmute = 0x50,
    */

    chat = 0x50,
    /*
    say = 0x10,
    whisper = 0x20
    */

    // Get stuff
    give = 0x60,

    // Commands to be overloaded
    update = 0x70,
    setup = 0x80
};

enum class SubCommand : unsigned char
{
    option1 = 0x10,
    option2 = 0x20,
    option3 = 0x30
};

}

#endif
