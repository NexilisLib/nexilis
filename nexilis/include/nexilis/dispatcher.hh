#ifndef NEXILIS_DISPATCHER_HH
#define NEXILIS_DISPATCHER_HH

#include <cstdint>
#include <nexilis/af_inet/udp_sender.hh>
#include <nexilis/boost/boost_udp_sender.hh>
#include <nexilis/connection.hh>
#include <nexilis/websocket/websocket_macros.hh>

namespace nexilis
{

class Dispatcher
{
public:
    /// Sends UDP message to a connection as a string.
    static void sendUDPMessage(Connection& connection, unsigned port, const std::string& message)
    {
        UDPSender sender(connection.getIPAddress().c_str(), port);
        sender.sendMessage(message);
    }

    static void sendUDPMessage(Connection& connection, unsigned port, const uint8_t* data, size_t dataSize)
    {
        UDPSender sender(connection.getIPAddress().c_str(), port);
        sender.sendMessage(data, dataSize);
    }

    /// Sends UDP message to ip address.
    static void sendUDPMessage(const char* ip_address, const std::string& message)
    {
        UDPSender sender(ip_address);
        sender.sendMessage(message);
    }

    static void sendBoostUDPMessage(Connection& connection, const std::string& message)
    {
        BoostUDPSender sender(boost_io_context::getIOContext(), connection.getIPAddress());
        boost_io_context::start();
        sender.sendMessage(message);
    }

    static void sendWebsocketMessage(wpp_websocket websocket, wpp_connection connection, const std::string& message)
    {
        websocket.send(connection, message, websocketpp::frame::opcode::text);
    }
};

} // namespace nexilis

#endif
