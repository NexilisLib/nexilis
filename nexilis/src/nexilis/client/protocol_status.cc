#include <nexilis/client/protocol_status.hh>

namespace nexilis::client
{

std::string ProtocolUtils::getProtocolStatusAsString(ProtocolStatus protocolStatus)
{
    switch (protocolStatus)
    {
        case ProtocolStatus::connected:
            return "connected";
        case ProtocolStatus::switching_ports:
            return "switching_ports";
        case ProtocolStatus::connecting:
            return "connecting";
        case ProtocolStatus::error:
            return "error";
        case ProtocolStatus::undefined:
            return "undefined";
    }
    return "";
}

} // namespace nexilis::client
