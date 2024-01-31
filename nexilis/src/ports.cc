#include <nexilis/ports.hh>

namespace nexilis
{

const char* portToString(Port port)
{
    switch (port)
    {
        case Port::UDP:
            return "54200";
        case Port::Websocket:
            return "54201";
    }
    return "";
}

} // namespace nexilis
