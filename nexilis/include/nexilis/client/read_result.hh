#ifndef NEXILIS_CLIENT_READ_RESULT_HH
#define NEXILIS_CLIENT_READ_RESULT_HH

#include <optional>
#include <string_view>
#include <utility>

namespace nexilis::client
{

/// Result from ClientAPI::readMessage(const nx_data&).
enum class ReadResult
{
    /// Command success.
    success,

    /// Successfull failure.
    failure,

    /// Failure in parsing.
    parsing_failed,

    /// Failure during command execution.
    command_execution,

    /// Failure in json conversion.
    error_in_json_conversion,

    /// Missing action or other data.
    not_found,

    /// The input for command is not valid.
    invalid_input,

    /// The command usage is unauthorized.
    unauthorized,

    /// Client is not found in the correct room.
    client_missing_room,

    /// Missing feature.
    not_implemented,
};

constexpr std::pair<std::string_view, ReadResult> read_result_mappings[] = {
        {"success", ReadResult::success},
        {"failure", ReadResult::failure},
        {"parsing_failed", ReadResult::parsing_failed},
        {"command_execution", ReadResult::command_execution},
        {"not_found", ReadResult::not_found},
        {"invalid_input", ReadResult::invalid_input},
        {"unauthorized", ReadResult::unauthorized},
        {"client_missing_room", ReadResult::client_missing_room},
        {"not_implemented", ReadResult::not_implemented},
        {"error_in_json_conversion", ReadResult::error_in_json_conversion}};

constexpr std::optional<ReadResult> readResultFromString(std::string_view name) noexcept
{
    for (const auto& [n, r] : read_result_mappings)
        if (n == name)
            return r;
    return std::nullopt;
}

} // namespace nexilis::client

#endif
