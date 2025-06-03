#include <nexilis/server/config.hh>
#include <cstdint>

namespace nexilis::server
{

bool Config::m_bigEndian = false;

bool Config::isSystemBigEndian()
{
    union {
        uint16_t value;
        uint8_t bytes[2];
    } test = {0x0102};
    return test.bytes[0] == 0x01; // True if big-endian
}

} // namespace nexilis::server
