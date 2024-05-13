#ifndef NEXILIS_MESSAGE_HANDLER_HH
#define NEXILIS_MESSAGE_HANDLER_HH

#include <nexilis/authentication.hh>
#include <nexilis/user.hh>

#include <cstdint>
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
    class Message
    {
    public:
        Message(const std::string& address, const std::vector<uint8_t>& data, uint16_t port, User* user)
            : m_address(address),
              m_data(data),
              m_port(port),
              m_user(user)
        {
        }

        std::string getAddress()
        {
            return m_address;
        }

        std::vector<uint8_t> getData()
        {
            return m_data;
        }

        uint16_t getPort()
        {
            return m_port;
        }

        User* getClient()
        {
            return m_user;
        }

    private:
        std::string m_address;
        std::vector<uint8_t> m_data;
        uint16_t m_port = 0;
        User* m_user = nullptr;
    };

    /// Read the unifiltered server message and return it ready for `Command`.
    /// /// \param address The incoming message sender address.
    /// /// \param message The incoming message data.
    /// /// \param port The incoming message sender port.
    /// /// \param authentication The server authentication levels.
    Message readMessage(std::string address, const std::vector<uint8_t>& payload, uint16_t port, Authentication* authentication);
};

} // namespace nexilis

#endif
