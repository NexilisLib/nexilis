#ifndef NEXILIS_MESSAGE_HANDLER_HH
#define NEXILIS_MESSAGE_HANDLER_HH

#include <nexilis/authentication.hh>
#include <nexilis/client.hh>

#include <vector>

namespace nexilis
{

/// MessageHandler
/// This class receives a server message from server `Protocol`.
/// and returns a Message object back to the protocol.
///
/// `Protocol` then sends the `MessageHandler::Message` to `Command` for parsing.
///
/// `MessageHandler::Message.message` is nexilis bytevector containing pure command data.
class MessageHandler
{
public:
    /// The object we are sending is done using C-style construction.
    /// TODO R5
    struct Message
    {
        std::string address;
        std::vector<uint8_t> message;
        uint16_t port = 0;
        Client* client = nullptr;
    };

    /// Read the unifiltered server message and return it ready for `Command`.
    /// /// \param address The incoming message sender address.
    /// /// \param message The incoming message data.
    /// /// \param port The incoming message sender port.
    /// /// \param authentication The server authentication levels.
    Message readMessage(std::string address, std::string message, uint16_t port, Authentication* authentication);
};

}

#endif
