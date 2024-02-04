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

    Protocol() :
        m_port(static_cast<uint32_t>(-1))
    {
    }

    Protocol(uint32_t port)
        : m_port(port)
    {
    }
    
    virtual ~Protocol() = default;

    virtual void start() = 0;

    // Should be made pure virtual.
    virtual void stop()
    {
    }

    virtual Type getType() = 0;

    uint32_t getPort() const
    {
        return m_port;
    };

protected:
    MessageHandler& getMessageHandler()
    {
        return m_messageHandler;
    }

private:
    // Internally -1 if protocol does not need port.
    uint32_t m_port;
    MessageHandler m_messageHandler;
};

} // namespace nexilis

#endif
