#ifndef NEXILIS_SERVER_PROTOCOL_HH
#define NEXILIS_SERVER_PROTOCOL_HH

#include <nexilis/message_handler.hh>
#include <nexilis/log.hh>

namespace nexilis
{

class ServerProtocol
{
public:
    // Default constructor.
    ServerProtocol() = default;

    /// Move constructor.
    ServerProtocol(ServerProtocol&& other);

    /// Move assignment operator.
    ServerProtocol& operator=(ServerProtocol&& other);

    /// Deleted copy constructor.
    ServerProtocol(const ServerProtocol&) = delete;

    /// Deleted copy assignment operator.
    ServerProtocol& operator=(const ServerProtocol&) = delete;

    virtual bool sendMessageToAll()
    {
        Log::error("Send message to all not implemented error");
        return false;
    }

protected:
    /// Use this to parse the message before sending to Command.
    MessageHandler& getMessageHandler()
    {
        return m_messageHandler;
    }

private:
    MessageHandler m_messageHandler;
};

} // namespace nexilis

#endif
