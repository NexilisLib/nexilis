#ifndef NEXILIS_SERVER_MESSAGE_ERROR_MESSAGE_HH
#define NEXILIS_SERVER_MESSAGE_ERROR_MESSAGE_HH

#include <nexilis/server/message/base_message.hh>

namespace nexilis::server
{

class ErrorMessage : public BaseMessage
{
public:
    enum class Type
    {
        client_id_failure,
        empty_authentication_mode,
        missing_authentication_mode,
        authentication_error,
        not_implemented,
        no_access
    };

    /// Constructor.
    ErrorMessage(const std::string& address, Type errorType);

    /// Deleted copy constructor.
    ErrorMessage(const ErrorMessage&) = delete;

    /// Deleted copy assignment operator.
    ErrorMessage& operator=(const ErrorMessage&) = delete;

    /// Move constructor.
    ErrorMessage(ErrorMessage&& other) noexcept;

    /// Move assignment operator.
    ErrorMessage& operator=(ErrorMessage&& other) noexcept;

    /// Type info.
    BaseMessage::Type getType() override
    {
        return BaseMessage::Type::error_message;
    }

private:
    Type m_errorType;
};

} // namespace nexilis::server

#endif
