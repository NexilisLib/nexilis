#ifndef NEXILIS_PORTS_HH
#define NEXILIS_PORTS_HH

#include <cstdint>

namespace nexilis
{

enum class Port : uint16_t
{
    UDP = 54200,
    Websocket = 54201
};

}

#endif
