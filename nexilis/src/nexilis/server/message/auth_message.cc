#include <nexilis/server/message/auth_message.hh>

namespace nexilis::server
{

AuthMessage::AuthMessage(BaseMessage::Data&& baseData, const std::vector<nx_data>& data)
    : BaseMessage(std::move(baseData)),
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
        m_data = std::move(other.m_data);
        BaseMessage::operator=(std::move(other));
    }
    return *this;
}

} // namespace nexilis::server
