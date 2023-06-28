#ifndef NEXILIS_WEBSOCKET_WEBSOCKETPP
#define NEXILIS_WEBSOCKET_WEBSOCKETPP

#include "websocket_macros.hh"

#include <functional>

namespace nexilis
{

class Websocketpp
{
public:
    /// Constructor.
    /// \param port Port for the websocket connection.
    Websocketpp(short port) :
        m_port(port)
    {
    }

    short getPort() { return m_port; }

private:
    short m_port;
};

}

#endif
