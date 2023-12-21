#ifndef NEXILIS_PROTOCOL_HH
#define NEXILIS_PROTOCOL_HH

#include <cstdint>

namespace nexilis
{

class Protocol
{
public:
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

protected:
    uint32_t getPort() const
    {
        return m_port;
    };

private:
    uint32_t m_port;
};

} // namespace nexilis

#endif
