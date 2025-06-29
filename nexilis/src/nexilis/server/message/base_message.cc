#include <nexilis/server/message/base_message.hh>

namespace nexilis::server
{

BaseMessage::BaseMessage(uint64_t messageId, const std::string& address, uint16_t port, User* user)
    : m_messageId(messageId), m_address(address), m_port(port), m_user(user)
{
}

BaseMessage::BaseMessage(BaseMessage&& other) noexcept
    : m_messageId(std::move(other.m_messageId)),
      m_address(std::move(other.m_address)),
      m_port(std::move(other.m_port)),
      m_user(other.m_user)
{
    other.m_user = nullptr;
}

BaseMessage& BaseMessage::operator=(BaseMessage&& other) noexcept
{
    if (this != &other)
    {
        m_messageId = std::move(other.m_messageId);
        m_address = std::move(other.m_address);
        m_port = std::move(other.m_port);

        m_user = other.m_user;
        other.m_user = nullptr;
    }
    return *this;
}

uint64_t BaseMessage::getMessageId() const
{
    return m_messageId;
}
const std::string& BaseMessage::getAddress() const
{
    return m_address;
}
uint16_t BaseMessage::getPort() const
{
    return m_port;
}
User* BaseMessage::getUser()
{
    return m_user;
}

} // namespace nexilis::server
