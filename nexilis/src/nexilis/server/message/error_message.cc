#include <nexilis/server/message/error_message.hh>

namespace nexilis::server
{

ErrorMessage::ErrorMessage(const std::string& address, Type errorType)
    : BaseMessage(BaseMessage::Data(0, address, nullptr)),
      m_errorType(errorType)
{
}

ErrorMessage::ErrorMessage(ErrorMessage&& other) noexcept
    : BaseMessage(std::move(other)),
      m_errorType(std::move(other.m_errorType))
{
}

ErrorMessage& ErrorMessage::operator=(ErrorMessage&& other) noexcept
{
    if (this != &other)
    {
        m_errorType = std::move(other.m_errorType);
        BaseMessage::operator=(std::move(other));
    }
    return *this;
}

} // namespace nexilis::server
