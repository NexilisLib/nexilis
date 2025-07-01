#ifndef NEXILIS_SERVER_MESSAGE_MESSAGE_HH
#define NEXILIS_SERVER_MESSAGE_MESSAGE_HH

#include <nexilis/server/message/base_message.hh>

namespace nexilis::server
{

class Message : public BaseMessage
{
public:
    /// Constructor.
    Message(BaseMessage::Data&& baseData, const nx_data& data);

    /// Deleted copy constructor.
    Message(const Message&) = delete;

    /// Deleted copy assignment operator.
    Message& operator=(const Message&) = delete;

    /// Move constructor.
    Message(Message&& other) noexcept;

    /// Move assignment operator.
    Message& operator=(Message&& other) noexcept;

    /// Type info.
    BaseMessage::Type getType() override
    {
        return BaseMessage::Type::message;
    }

    /// Get the message data.
    nx_data getData() const;

private:
    nx_data m_data;
};

} // namespace nexilis::server

#endif
