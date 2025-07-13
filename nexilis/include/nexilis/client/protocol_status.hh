#ifndef NEXILIS_PROTOCOL_STATUS_HH
#define NEXILIS_PROTOCOL_STATUS_HH

#include <string>

namespace nexilis::client
{

enum class ProtocolStatus
{
    undefined,
    connecting,
    connected,
    switching_ports,
    error
};

class ProtocolUtils
{
public:
    static std::string getProtocolStatusAsString(ProtocolStatus protocolStatus);
};

} // namespace nexilis::client

#endif
