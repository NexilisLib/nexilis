#ifndef NEXILIS_WEBSOCKET_DISPATCHER_HH
#define NEXILIS_WEBSOCKET_DISPATCHER_HH

#include "connection.hh"
#include "udp/unix_udp_sender.hh"
#include "udp/boost_udp_sender.hh"
#include "ports.hh"
#include <boost/asio/io_context.hpp>

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
        boost::asio::io_context io_context;
        BoostUDPSender sender(io_context, connection.getIPAddress());
        sender.sendMessage(message);
        io_context.run();
    }

};

}

#endif
