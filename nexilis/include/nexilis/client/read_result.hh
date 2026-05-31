#ifndef NEXILIS_CLIENT_READ_RESULT_HH
#define NEXILIS_CLIENT_READ_RESULT_HH

namespace nexilis::client
{

/// Result from ClientAPI::readMessage(const nx_data&).
enum class ReadResult
{
    /// Command success.
    success,

    /// Client experiences a failure in executing the command.
    failure,

    // Failure in parsing.
    parsing_failed,

    // TODO
    // command_generation,

    /// Something wrong with the command data.
    not_found,

    /// The input for command is not valid.
    invalid_input,

    /// The command usage is unauthorized.
    unauthorized,

    /// Client is not found in the correct room.
    client_missing_room,

    /// Missing feature.
    not_implemented,

    /// Anything else
    error
};

} // namespace nexilis::client

#endif
