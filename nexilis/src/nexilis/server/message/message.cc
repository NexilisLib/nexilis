#include <nexilis/server/message/message.hh>

namespace nexilis::server
{

Message::Message(BaseMessage&& baseMessage, const nx_data& data)
    : BaseMessage(std::move(baseMessage)),
      m_data(data)
{
}

Message::Message(Message&& other) noexcept
    : BaseMessage(std::move(other)),
      m_data(std::move(other.m_data))
{
}

Message& Message::operator=(Message&& other) noexcept
{
    if (this != &other)
    {
        BaseMessage::operator=(std::move(other));
        m_data = std::move(other.m_data);
    }
    return *this;
}

nx_data Message::getData() const
{
    return m_data;
}

} // namespace nexilis::server
