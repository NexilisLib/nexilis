#ifndef NEXILIS_PROTOCOL_HH
#define NEXILIS_PROTOCOL_HH

#include <cstdint>

namespace nexilis
{

// Forward declarations of inherited class might be necessary here.

class Protocol
{
public:
    // These are the types inherited from this class.
    enum class Type
    {
        UDP,
        Websocket,
        UnixSocket
    };

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

protected:
    uint32_t getPort() const
    {
        return m_port;
    };

private:
    // Internally -1 if protocol does not need port.
    uint32_t m_port;
};

} // namespace nexilis

#endif
