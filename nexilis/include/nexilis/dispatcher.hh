#ifndef NEXILIS_WEBSOCKET_DISPATCHER_HH
#define NEXILIS_WEBSOCKET_DISPATCHER_HH

#include "connection.hh"
#include "udp/udp_sender.hh"

namespace nexilis
{

class Dispatcher
{
public:
    // self
    static void sendWebsocketMessage(const Connection& connection, const std::string& message)
    {
        connection.getWppServer().send(connection.getWppConnection(), message, websocketpp::frame::opcode::text);
    }

    // self
    static void sendUDPMessage(Connection& connection, const std::string& message)
    {
        UDPSender sender(connection.getIPAddress(), connection.getPortNumber());
        sender.sendMessage(message);
    }
};

}

#endif
