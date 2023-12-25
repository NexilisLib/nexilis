#ifndef NEXILIS_PORTS_HH
#define NEXILIS_PORTS_HH

namespace nexilis
{

// These ports define the default port values for different "protocols".
enum class Port
{
    UDP = 54200,
    Websocket = 54201
};

const char* portToString(Port port);

} // namespace nexilis

#endif
