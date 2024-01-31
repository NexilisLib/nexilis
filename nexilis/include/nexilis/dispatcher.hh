#ifndef NEXILIS_DISPATCHER_HH
#define NEXILIS_DISPATCHER_HH

#include <cstdint>
#include <nexilis/common/af_inet_udp_sender.hh>
#include <nexilis/client.hh>

namespace nexilis
{

class Dispatcher
{
public:
    /// Sends UDP message to a connection as a string.
    static void sendUDPMessage(Client& client, unsigned port, const std::string& message)
    {
        AfInetUdpSender sender(client.getIPAddress().c_str(), port);
        sender.sendMessage(message);
    }

    static void sendUDPMessage(Client& client, unsigned port, const uint8_t* data, size_t dataSize)
    {
        AfInetUdpSender sender(client.getIPAddress().c_str(), port);
        sender.sendMessage(data, dataSize);
    }

    /// Sends UDP message to ip address.
    static void sendUDPMessage(const char* ip_address, const std::string& message)
    {
        AfInetUdpSender sender(ip_address);
        sender.sendMessage(message);
    }
};

} // namespace nexilis

#endif
