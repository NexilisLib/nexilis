#include <nexilis/ports.hh>

namespace nexilis
{

uint16_t Ports::m_boostTCPPort = 54200;

uint16_t Ports::getBoostTCPPort()
{
    return m_boostTCPPort;
}

} // namespace nexilis
