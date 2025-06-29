#ifndef NEXILIS_MESSAGE_BASE_MESSAGE_HH
#define NEXILIS_MESSAGE_BASE_MESSAGE_HH

#include <nexilis/server/user.hh>

namespace nexilis::server
{

class BaseMessage
{
public:
    /// Constructor.
    explicit BaseMessage(uint64_t messageId, const std::string& address, uint16_t port, User* user);

    /// Deleted copy constructor.
    BaseMessage(const BaseMessage&) = delete;

    /// Deleted copy assignment operator.
    BaseMessage& operator=(const BaseMessage&) = delete;

    /// Move constructor.
    BaseMessage(BaseMessage&& other) noexcept;

    /// Move assignment operator.
    BaseMessage& operator=(BaseMessage&& other) noexcept;

    /// Virtual destructor.
    virtual ~BaseMessage() = default;

    /// Get the unique identifier of the message.
    uint64_t getMessageId() const;

    /// Get the address where the message is assigned.
    const std::string& getAddress() const;

    /// Get the port number where the message is assigned.
    uint16_t getPort() const;

    /// Get pointer to the user that is associated with the message.
    User* getUser();

private:
    uint64_t m_messageId;
    std::string m_address;
    uint16_t m_port = 0;
    User* m_user = nullptr;
};

} // namespace nexilis::server

#endif
