#ifndef NEXILIS_UDP_SENDER_HH
#define NEXILIS_UDP_SENDER_HH

#include <nexilis/af_inet/udp_server.hh>
#include <nexilis/ports.hh>

#include <arpa/inet.h>

#include <string>

namespace nexilis
{

class AfInetUdpSender
{
public:
    /// Constructor.
    /// \param destinationIP The IP address where the message(s) will be sent.
    AfInetUdpSender(const char* destinationIP, unsigned destinationPort = static_cast<unsigned>(Port::UDP));

    // Destructor.
    ~AfInetUdpSender();

    /// Send message to to destination.
    /// \param message The message to be sent.
    void sendMessage(const std::string& message);

    // Send message as bytes to the destination.
    // \param data The message to be sent.
    // \param dataSize The size of the data to be sent.
    void sendMessage(const unsigned char* data, size_t dataSize);

private:
    int m_socket;
    const char* m_destinationIP;
    sockaddr_in m_destinationAddress;

    unsigned short m_destinationPort;
};

} // namespace nexilis

#endif
