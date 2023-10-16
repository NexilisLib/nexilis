#ifndef NEXILIS_DISPATCHER_HH
#define NEXILIS_DISPATCHER_HH

#include "connection.hh"
#include "af_inet/unix_udp_sender.hh"
#include "boost/boost_udp_sender.hh"
#include "boost/boost_io_context.hh"

#include "ports.hh"

namespace nexilis
{

class Dispatcher
{
public:
    /// Sends websocket message to a connection.
    static void sendWebsocketMessage(const Connection& connection, const std::string& message)
    {
        connection.getWppServer().send(connection.getWppConnection(), message, websocketpp::frame::opcode::text);
    }

    /// Sends UDP message to a connection.
    static void sendUDPMessage(Connection& connection, const std::string& message)
    {
        UnixUDPSender sender(connection.getIPAddress().c_str());
        sender.sendMessage(message);
    }

    /// Sends UDP message to ip address.
    static void sendUDPMessage(const char* ip_address, const std::string& message)
    {
        UnixUDPSender sender(ip_address);
        sender.sendMessage(message);
    }

    static void sendBoostUDPMessage(Connection& connection, const std::string& message)
    {
        BoostUDPSender sender(boost_io_context::getIOContext(), connection.getIPAddress());
        boost_io_context::start();
        sender.sendMessage(message);
    }
};

}

#endif
