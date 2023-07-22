#ifndef NEXILIS_WEBSOCKET_DISPATCHER_HH
#define NEXILIS_WEBSOCKET_DISPATCHER_HH

#include "websocket/websocket_macros.hh"
#include "connection.hh"

namespace nexilis
{

class Dispatcher
{
public:
    static void sendMessage(wpp_websocket websocket, wpp_connection connection, const std::string& message)
    {
        websocket.send(connection, message, websocketpp::frame::opcode::text);
    }

};

}

#endif
