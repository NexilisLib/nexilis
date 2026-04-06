#ifndef NEXILIS_SERVER_COMMAND_RESULT_HH
#define NEXILIS_SERVER_COMMAND_RESULT_HH

#include <string>

namespace nexilis::server
{

enum class CommandResult
{
    /// Unimplemented actions.
    unimplemented,

    // Command success.
    success,

    // Logical failure in the command, failing is ok.
    failure,

    // Command is not found.
    not_found,

    // The input for command is not correct.
    invalid_input,

    // There is an error implementing command.
    error,

    // The command usage is unauthorized.
    unauthorized,

    // Command failed in sending the room data command
    failed_room_send
};

std::string asString(CommandResult res);
bool checkResult(CommandResult res);

} // namespace nexilis::server

#endif
