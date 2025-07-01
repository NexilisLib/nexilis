#ifndef NEXILIS_MESSAGE_BASE_MESSAGE_HH
#define NEXILIS_MESSAGE_BASE_MESSAGE_HH

#include <nexilis/server/user.hh>

namespace nexilis::server
{

/// Base class for incoming messages from the client.
class BaseMessage
{
public:
    enum Type
    {
        message,
        auth_message,
        error_message
    };

    class Data
    {
    public:
        /// Constructor.
        explicit Data(uint64_t messageId, const std::string& address, uint16_t port, User* user);

        /// Deleted copy constructor.
        Data(const Data&) = delete;

        /// Deleted copy assignment operator.
        Data& operator=(const Data&) = delete;

        /// Move constructor.
        Data(Data&& other) noexcept;

        /// Move assignment operator.
        Data& operator=(Data&& other) noexcept;

        uint64_t getMessageId() const;

        const std::string& getAddress() const;

        // TODO This is bad, not all protocols have ports.
        uint16_t getPort() const;

        User* getUser() const;

    private:
        uint64_t m_messageId;
        std::string m_address;
        uint16_t m_port = 0;
        User* m_user = nullptr;
    };

    /// Constructor.
    explicit BaseMessage(Data&& data);

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

    /// Get the type of the class that inherits BaseMessage.
    virtual Type getType() = 0;

    /// Get the unique identifier of the message.
    uint64_t getMessageId() const;

    /// Get the address where the message is assigned.
    const std::string& getAddress() const;

    /// Get the port number where the message is assigned.
    uint16_t getPort() const;

    /// Get pointer to the user that is associated with the message.
    User* getUser() const;

private:
    Data m_data;
};

} // namespace nexilis::server

#endif
