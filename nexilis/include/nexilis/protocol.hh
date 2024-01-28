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
        UDP,
        TCP,
        Websocket,
        UnixSocket
    };

    Protocol() :
        m_port(static_cast<uint32_t>(-1))
    {
    }

    Protocol(uint32_t port)
        : m_port(port)
    {
    }

    virtual void start()
    {
    }

    virtual void stop()
    {
    }

    virtual Type getType() = 0;

    uint32_t getPort() const
    {
        return m_port;
    };

protected:
    MessageHandler getMessageHandler()
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
