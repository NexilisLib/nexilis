#include <nexilis/server/message/message.hh>

namespace nexilis::server
{

Message::Message(BaseMessage::Data&& baseData, const nx_data& data)
    : BaseMessage(std::move(baseData)),
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
        m_data = std::move(other.m_data);
        BaseMessage::operator=(std::move(other));
    }
    return *this;
}

nx_data Message::getData() const
{
    return m_data;
}

} // namespace nexilis::server
