#ifndef NEXILIS_COMMAND_HH
#define NEXILIS_COMMAND_HH

#include <nexilis/authentication.hh>
#include <nexilis/command_type.hh>
#include <nexilis/protocol.hh>
#include <nexilis/json.hh>

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace nexilis
{

/// Nexilis Server-side API.
/// Command contains static reading functions for the nexilis byte sequence.
/// These bytes have been cleared from MessageHandler and contains vector<uint8>& which triggers all the actions of nexilis.
class Command
{
public:
    /// Result for reading the Nexilis command sequence.
    enum class Result
    {
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
        unauthorized
    };

    /// Read the command from client.
    /// \param command The vector of bytes that is the command.
    /// \param user The user that sent the message.
    /// \param protocol The protocol that was used in receiving the message.
    /// \param messageId The unique identifier for the message.
    /// \return Result from reading the command.
    static Result read(const std::vector<uint8_t>& command, User& user, Protocol& protocol, uint64_t messageId);

    /// Read the command from client.
    /// \param command_data The data for the command
    /// \param lenght The command lenght in bytes.
    /// \param user The user that sent the message.
    /// \param protocol The protocol that was used in receiving the message.
    /// \param messageId The unique identifier for the message.
    /// \return Result from reading the command.
    static Result read(const char* command_data, size_t lenght, User& client, Protocol& protocol, uint64_t messageId);

    /// Give server authentication details via Authentication object.
    /// /// \param authentication The object than contains authentication rules.
    static void setAuthentication(Authentication& authentication)
    {
        m_authentication = &authentication;
    }

    /// Get authentication details.
    static Authentication* getAuthentication()
    {
        return m_authentication;
    }

    /// Helper functions.

    /// Create IPv4 address from IPV4 data.
    /// \param address The address data.
    /// \return string Created IPV4 string.
    std::string createIPv4Address(const std::vector<uint8_t>& address);

private:
    /// Send message to every protocol that is avainable for a client;
    static void sendMessageToClient(std::vector<uint8_t> data, User& user, Protocol& protocol);

private:
    static Authentication* m_authentication;
};

} // namespace nexilis

#endif
