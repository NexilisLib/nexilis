#ifndef NEXILIS_SERVER_PROTOCOL_HH
#define NEXILIS_SERVER_PROTOCOL_HH

#include <nexilis/logger/log.hh>
#include <nexilis/server/command.hh>
#include <nexilis/server/message_handler.hh>
#include <nexilis/server/settings.hh>

namespace nexilis::server
{

class ServerProtocol
{
public:
    // Default constructor.
    ServerProtocol(const Settings& settings);

    /// Move constructor.
    ServerProtocol(ServerProtocol&& other);

    /// Move assignment operator.
    ServerProtocol& operator=(ServerProtocol&& other);

    /// Deleted copy constructor.
    ServerProtocol(const ServerProtocol&) = delete;

    /// Deleted copy assignment operator.
    ServerProtocol& operator=(const ServerProtocol&) = delete;

protected:
    /// Use this to parse the message before sending to Command.
    MessageHandler& getMessageHandler()
    {
        return m_messageHandler;
    }

    const MessageHandler& getMessageHandler() const
    {
        return m_messageHandler;
    }

    Command& getCommand()
    {
        return m_command;
    }

    const Command& getCommand() const
    {
        return m_command;
    }

private:
    MessageHandler m_messageHandler;
    Command m_command;
};

} // namespace nexilis::server

#endif
