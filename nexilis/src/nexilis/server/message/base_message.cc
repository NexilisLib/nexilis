#include <nexilis/server/message/base_message.hh>

namespace nexilis::server
{

BaseMessage::Data::Data(uint64_t messageId, const std::string& address, uint16_t port, User* user)
    : m_messageId(messageId),
      m_address(address),
      m_port(port),
      m_user(user)
{
}

BaseMessage::Data::Data(Data&& other) noexcept
    : m_messageId(std::move(other.m_messageId)),
      m_address(std::move(other.m_address)),
      m_port(std::move(other.m_port)),
      m_user(std::move(other.m_user))
{
}

BaseMessage::Data& BaseMessage::Data::operator=(Data&& other) noexcept
{
    if (this != &other)
    {
        m_messageId = std::move(other.m_messageId);
        m_address = std::move(other.m_address);
        m_port = std::move(other.m_port);
        m_user = std::move(other.m_user);
        other.m_user = nullptr;
    }
    return *this;
}

uint64_t BaseMessage::Data::getMessageId() const
{
    return m_messageId;
}

const std::string& BaseMessage::Data::getAddress() const
{
    return m_address;
}

uint16_t BaseMessage::Data::getPort() const
{
    return m_port;
}

User* BaseMessage::Data::getUser() const
{
    return m_user;
}

BaseMessage::BaseMessage(Data&& data)
    : m_data(std::move(data))
{
}

BaseMessage::BaseMessage(BaseMessage&& other) noexcept
    : m_data(std::move(other.m_data))
{
}

BaseMessage& BaseMessage::operator=(BaseMessage&& other) noexcept
{
    if (this != &other)
    {
        m_data = std::move(other.m_data);
    }
    return *this;
}

uint64_t BaseMessage::getMessageId() const
{
    return m_data.getMessageId();
}

const std::string& BaseMessage::getAddress() const
{
    return m_data.getAddress();
}

uint16_t BaseMessage::getPort() const
{
    return m_data.getPort();
}

User* BaseMessage::getUser() const
{
    return m_data.getUser();
}

} // namespace nexilis::server
