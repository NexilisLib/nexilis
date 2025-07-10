#ifndef NEXILIS_PROTOCOL_STATUS_HH
#define NEXILIS_PROTOCOL_STATUS_HH

namespace nexilis
{

enum class ProtocolStatus
{
    undefined,
    connecting,
    connected,
    switching_ports,
    error
};

}

#endif
