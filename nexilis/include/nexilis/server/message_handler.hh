#ifndef NEXILIS_MESSAGE_HANDLER_HH
#define NEXILIS_MESSAGE_HANDLER_HH

#include <nexilis/nx_class.hh>
#include <nexilis/server/settings.hh>
#include <nexilis/server/user.hh>

#include <cstdint>

namespace nexilis::server
{

/// MessageHandler
/// This class receives a server message from server `Protocol`.
/// and returns a Message object back to the protocol.
///
/// `Protocol` then sends the `MessageHandler::Message` to `Command` for parsing.
///
/// `MessageHandler::Message.message` is nexilis bytevector containing pure command data.

class MessageHandler : public NxClass
{
public:
    /// Default constructor.
    MessageHandler();

    class Message
    {
    public:
        Message(const std::string& address, const nx_data& data, uint16_t port, User* user, uint64_t messageId)
            : m_address(address),
              m_data(data),
              m_port(port),
              m_user(user),
              m_messageId(messageId)
        {
        }

        const std::string& getAddress() const
        {
            return m_address;
        }

        nx_data getData() const
        {
            return m_data;
        }

        uint16_t getPort() const
        {
            return m_port;
        }

        User* getClient()
        {
            return m_user;
        }

        uint64_t getMessageId() const
        {
            return m_messageId;
        }

    private:
        std::string m_address;
        nx_data m_data;
        uint16_t m_port = 0;
        User* m_user = nullptr;
        uint64_t m_messageId;
    };

    /// Read the unifiltered server message and return it ready for `Command`.
    /// \param address The incoming message sender address.
    /// \param message The incoming message data.
    /// \param port The incoming message sender port.
    /// \param authentication The server authentication levels.
    Message readMessage(std::string address, const nx_data& payload, uint16_t port, Settings* authentication);

private:
    Message handlePayload(const nx_data& payload, User* user, const std::string& address, uint16_t port);
};

} // namespace nexilis::server

#endif
