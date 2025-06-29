#ifndef NEXILIS_MESSAGE_HANDLER_HH
#define NEXILIS_MESSAGE_HANDLER_HH

#include <nexilis/nx_class.hh>
#include <nexilis/server/settings.hh>

#include <nexilis/server/message/message.hh>

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
