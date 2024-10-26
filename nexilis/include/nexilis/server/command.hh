#ifndef NEXILIS_COMMAND_HH
#define NEXILIS_COMMAND_HH

#include <nexilis/server/settings.hh>
#include <nexilis/server/user.hh>
#include <nexilis/command_type.hh>
#include <nexilis/protocol.hh>

#include <cstddef>
#include <cstdint>
#include <string>
#include <map>
#include <thread>

namespace nexilis::server
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
    /// \param settings The settings of the server.
    Command(const Settings& settings);

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
    Result read(const nx_data& command, User& user, Protocol& protocol, uint64_t messageId);

    /// Read the command from client.
    /// \param command_data The data for the command
    /// \param length The command length in bytes.
    /// \param user The user that sent the message.
    /// \param protocol The protocol that was used in receiving the message.
    /// \param messageId The unique identifier for the message.
    /// \return Result from reading the command.
    Result read(const char* command_data, size_t length, User& client, Protocol& protocol, uint64_t messageId);

    Settings& getSettings()
    {
        return m_settings;
    }

    const Settings& getSettings() const
    {
        return m_settings;
    }
private:
    /// Send message to every protocol that is avainable for a client;
    void sendMessageToClient(nx_data data, User& user, Protocol& protocol);

    nx_data createRoomCommand(uint64_t roomId, User& user, const nx_data& messageData, const std::map<std::string, boost::json::value>& params, uint64_t messageId);
    void sendRoomCommand(const nx_data& data, User& user, Protocol& protocol);

    /// Send multiple messages with specified tickrate.
    void runWithTickrate(double tickrate, double durationSeconds, const std::function<void(double)>& tickFunction);

    // Smooth movement.
    double easing(double progress, double totalDistance);

    // Linear movement.
    double linear(double progress, double totalDistance);

    struct Object2DMovementParams
    {
        uint64_t objectId;
        Vector2f movementAmount;
        float deltaTime;
        std::function<double(double, double)> movementFunction;
        nx_data messageData;
        uint64_t messageId;
    };

    std::thread object2DMovement(const Object2DMovementParams& params, User& user, Protocol& protocol);

private:
    /// The "settings" of the server protocol.
    Settings m_settings;
};

} // namespace nexilis::server

#endif
