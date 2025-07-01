#ifndef NEXILIS_SERVER_MESSAGE_AUTH_MESSAGE_HH
#define NEXILIS_SERVER_MESSAGE_AUTH_MESSAGE_HH

#include <nexilis/server/message/base_message.hh>

namespace nexilis::server
{

class AuthMessage : public BaseMessage
{
public:
    /// Constructor.
    AuthMessage(BaseMessage&& baseMessage, const std::vector<nx_data>& data);

    /// Deleted copy constructor.
    AuthMessage(const AuthMessage&) = delete;

    /// Deleted copy assignment operator.
    AuthMessage& operator=(const AuthMessage&) = delete;

    /// Move constructor.
    AuthMessage(AuthMessage&& other) noexcept;

    /// Move assignment operator.
    AuthMessage& operator=(AuthMessage&& other) noexcept;

    /// Type info.
    BaseMessage::Type getType() override
    {
        return BaseMessage::Type::auth_message;
    }

private:
    std::vector<nx_data> m_data;
};

} // namespace nexilis::server

#endif
