#ifndef NEXILIS_COMMAND_HH
#define NEXILIS_COMMAND_HH

#include <nexilis/command_type.hh>
#include <nexilis/movement/movement_2D.hh>
#include <nexilis/movement/movement_3D.hh>
#include <nexilis/nx_class.hh>
#include <nexilis/protocol.hh>
#include <nexilis/server/command/command_result.hh>
#include <nexilis/server/settings.hh>
#include <nexilis/server/user.hh>

#include <cstddef>
#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <thread>

namespace nexilis::server
{

/// Nexilis Server-side API.
/// Command contains functionality for reading nexilis byte sequence.
/// These bytes have been cleared from MessageHandler and contains vector<uint8>& which triggers all the actions of nexilis.
class Command : public NxClass
{
public:
    using ClientMsgType = std::map<std::string, boost::json::value>;

    /// Constructor.
    /// \param settings The settings of the server.
    explicit Command(const Settings& settings);

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
    CommandResult read(const nx_data& command, User& user, Protocol& protocol, uint64_t messageId);

    /// Read the command from client.
    /// \param command_data The data for the command
    /// \param length The command length in bytes.
    /// \param user The user that sent the message.
    /// \param protocol The protocol that was used in receiving the message.
    /// \param messageId The unique identifier for the message.
    /// \return Result from reading the command.
    CommandResult read(const char* command_data, size_t length, User& client, Protocol& protocol, uint64_t messageId);

    Settings& getSettings()
    {
        return m_settings;
    }

    const Settings& getSettings() const
    {
        return m_settings;
    }

    // TODO create own interface for these static functions.
    static nx_data clientMessageData(CommandType cmd, const std::string& type, uint64_t message_id, const ClientMsgType& params);

    /// Send message to every protocol that is avainable for a client.
    static bool sendMessageToClient(nx_data data, User& user, Protocol& protocol);
    static bool sendRoomCommand(const nx_data& data, User& user, Protocol& protocol);
    static nx_data createRoomCommand(uint64_t roomId, User& user, const nx_data& messageData, const ClientMsgType& params, uint64_t messageId);

    /// Send multiple messages with specified tickrate.
    static void runWithTickrate(double tickrate, double durationSeconds, const std::function<void(double)>& tickFunction);

private:
    struct MovementParams
    {
        MovementData movement_data;
        User* user;
        Protocol& protocol;
        float tickrate;
        float delta_time;
    };

    // std::thread player2DMovement(Movement2D movement, const MovementParams& params);
    // std::thread player3DMovement(Movement3D movement, const MovementParams& params);

    static ClientMsgType clientMessageMap(CommandType cmd, const std::string& type, uint64_t message_id, const ClientMsgType& params);

private:
    /// The server side configuration.
    Settings m_settings;
};

} // namespace nexilis::server

#endif
