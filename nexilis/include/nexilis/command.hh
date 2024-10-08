#ifndef NEXILIS_COMMAND_HH
#define NEXILIS_COMMAND_HH

#include <nexilis/authentication.hh>
#include <nexilis/command_type.hh>
#include <nexilis/json.hh>
#include <nexilis/protocol.hh>

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace nexilis
{

/// Nexilis Server-side API.
/// Command contains functionality for reading nexilis byte sequence.
/// These bytes have been cleared from MessageHandler and contains vector<uint8>& which triggers all the actions of nexilis.
class Command
{
public:
    /// Result for reading the Nexilis command sequence.
    enum class Result
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
        unauthorized
    };

    /// Constructor.
    /// \param authentication The settings of the server.
    /// \param tickrate The tickrate of the server.
    Command(const Authentication& authentication);

    // Move constructor.
    Command(Command&& other);

    // Move assignment operator.
    Command& operator=(Command&& other);

    /// Deleted copy constructor.
    Command(const Command& other) = delete;

    /// Deleted copy assignment operator.
    Command& operator=(const Command& other) = delete;

    /// Read the command from client.
    /// \param command The vector of bytes that is the command.
    /// \param user The user that sent the message.
    /// \param protocol The protocol that was used in receiving the message.
    /// \param messageId The unique identifier for the message.
    /// \return Result from reading the command.
    Result read(const std::vector<uint8_t>& command, User& user, Protocol& protocol, uint64_t messageId);

    /// Read the command from client.
    /// \param command_data The data for the command
    /// \param length The command length in bytes.
    /// \param user The user that sent the message.
    /// \param protocol The protocol that was used in receiving the message.
    /// \param messageId The unique identifier for the message.
    /// \return Result from reading the command.
    Result read(const char* command_data, size_t length, User& client, Protocol& protocol, uint64_t messageId);

protected:
    Authentication& getAuthentication()
    {
        return m_authentication;
    }

    const Authentication& getAuthentication() const
    {
        return m_authentication;
    }
private:
    /// Send message to every protocol that is avainable for a client;
    void sendMessageToClient(std::vector<uint8_t> data, User& user, Protocol& protocol);

    std::vector<uint8_t> createRoomCommand(uint64_t roomId, User& user, const std::vector<uint8_t>& messageData, const std::map<std::string, boost::json::value>& params, uint64_t messageId);
    void sendRoomCommand(const std::vector<uint8_t>& data, User& user, Protocol& protocol);

    /// Send multiple messages with specified tickrate.
    void runWithTickrate(double tickrate, double durationSeconds, const std::function<void(double)>& tickFunction);
    double easing(double progress, double totalDistance);

private:
    /// The "settings" of the server protocol.
    Authentication m_authentication;
};

} // namespace nexilis

#endif
