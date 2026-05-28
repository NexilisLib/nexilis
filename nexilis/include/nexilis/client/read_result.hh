#ifndef NEXILIS_CLIENT_READ_RESULT_HH
#define NEXILIS_CLIENT_READ_RESULT_HH

namespace nexilis::client
{

/// Result from ClientAPI::readMessage(const nx_data&).
enum class ReadResult
{
    /// The payload has nothing with nexilis.
    clean,

    /// Command success.
    success,

    /// Allowed failure.
    failure,

    /// Something wrong with the command data.
    not_found,

    /// The input for command is not correct.
    invalid_input,

    /// Internal error.
    error,

    /// The command usage is unauthorized.
    unauthorized,

    /// Client is not found in the correct room.
    client_missing_room,

    /// Missing feature.
    not_implemented
};

} // namespace nexilis::client

#endif
