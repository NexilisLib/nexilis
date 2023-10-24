#ifndef NEXILIS_DISPATCHER_HH
#define NEXILIS_DISPATCHER_HH

#include <nexilis/af_inet/udp_sender.hh>
#include <nexilis/boost/boost_udp_sender.hh>
#include <nexilis/connection.hh>

namespace nexilis
{

class Dispatcher
{
public:
    /// Sends UDP message to a connection.
    static void sendUDPMessage(Connection& connection, const std::string& message)
    {
        UDPSender sender(connection.getIPAddress().c_str());
        sender.sendMessage(message);
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
};

} // namespace nexilis

#endif
