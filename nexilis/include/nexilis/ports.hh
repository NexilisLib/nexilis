#ifndef NEXILIS_PORTS_HH
#define NEXILIS_PORTS_HH

#include <cstdint>

namespace nexilis
{

/// This class holds the fixed port numbers for initial communication.
class Ports
{
public:
    static uint16_t getBoostTCPPort();

private:
    static uint16_t m_boostTCPPort;
};

} // namespace nexilis

#endif
