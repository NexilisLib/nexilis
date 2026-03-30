#ifndef NEXILIS_SERVER_PROTOCOL_HH
#define NEXILIS_SERVER_PROTOCOL_HH

#include <nexilis/logger/log.hh>
#include <nexilis/server/command/command.hh>
#include <nexilis/server/message/message_handler.hh>
#include <nexilis/server/settings.hh>

namespace nexilis::server
{

class ServerProtocol
{
public:
    // Constructor.
    explicit ServerProtocol(const Settings& settings);

    /// Move constructor.
    ServerProtocol(ServerProtocol&& other);

    /// Move assignment operator.
    ServerProtocol& operator=(ServerProtocol&& other);

    /// Deleted copy constructor.
    ServerProtocol(const ServerProtocol&) = delete;

    /// Deleted copy assignment operator.
    ServerProtocol& operator=(const ServerProtocol&) = delete;

    /// Get the settings for given server protocol.
    Settings& getSettings()
    {
        return m_command.getSettings();
    }

    /// Check if there are active connections.
    bool hasActiveConnections() const
    {
        return m_activeConnections > 0;
    }

    /// Get the count of active connections.
    uint64_t activeConnectionsCount() const
    {
        return m_activeConnections;
    }

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

    /// Derived class calls when a new connection is established.
    void connectionEstablished()
    {
        ++m_activeConnections;
    }

    /// Derived class calls when a connection is closed.
    void connectionClosed()
    {
        --m_activeConnections;
    }

private:
    MessageHandler m_messageHandler;
    Command m_command;

    /// Active connections for the server protocol.
    std::atomic<uint64_t> m_activeConnections{0};
};

} // namespace nexilis::server

#endif
