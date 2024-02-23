#ifndef NEXILIS_PROTOCOL_HH
#define NEXILIS_PROTOCOL_HH

#include <nexilis/message_handler.hh>

#include <cstdint>

namespace nexilis
{

class Protocol
{
public:
    // These are the types inherited from this class.
    enum class Type
    {
        AF_INET_UDP_SERVER,
        AF_INET_UDP_CLIENT,
        AF_INET_TCP_SERVER,
        AF_INET_TCP_CLIENT,

        BOOST_UDP_SERVER,
        BOOST_UDP_CLIENT,
        BOOST_TCP_SERVER,
        BOOST_TCP_CLIENT,

        AF_UNIX_SOCK_DGRAM_CLIENT,
        AF_UNIX_SOCK_DGRAM_SERVER,
        AF_UNIX_SOCK_STREAM_CLIENT,
        AF_UNIX_SOCK_STREAM_SERVER
    };

    /// Default constructor.
    Protocol() = default;

    /// Move constructor.
    Protocol(Protocol&& other);

    /// Move assignment operator.
    Protocol& operator=(Protocol&& other);

    /// Deleted copy constructor.
    Protocol(const Protocol& other) = delete;

    /// Deleted copy assignment operator.
    Protocol& operator=(const Protocol& other) = delete;
    
    virtual ~Protocol() = default;

    virtual void start() = 0;

    // Should be made pure virtual.
    virtual void stop()
    {
    }

    virtual Type getType() = 0;

protected:
    MessageHandler& getMessageHandler()
    {
        return m_messageHandler;
    }

    std::string logName()
    {
        return typeToString(getType()) + ": ";
    }

private:
    std::string typeToString(Type type);
private:
    MessageHandler m_messageHandler;
};

} // namespace nexilis

#endif
