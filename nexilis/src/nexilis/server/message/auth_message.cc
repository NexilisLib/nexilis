#include <nexilis/server/message/auth_message.hh>

namespace nexilis::server
{

AuthMessage::AuthMessage(BaseMessage&& baseMessage, const std::vector<nx_data>& data)
    : BaseMessage(std::move(baseMessage)),
      m_data(data)
{
}

AuthMessage::AuthMessage(AuthMessage&& other) noexcept
    : BaseMessage(std::move(other)),
      m_data(std::move(other.m_data))
{
}

AuthMessage& AuthMessage::operator=(AuthMessage&& other) noexcept
{
    if (this != &other)
    {
        BaseMessage::operator=(std::move(other));
        m_data = std::move(other.m_data);
    }
    return *this;
}

} // namespace nexilis::server
