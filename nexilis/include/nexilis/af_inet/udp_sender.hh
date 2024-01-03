#ifndef NEXILIS_UDP_SENDER_HH
#define NEXILIS_UDP_SENDER_HH

#include <nexilis/ports.hh>

#include <arpa/inet.h>

#include <string>

namespace nexilis
{

class UDPSender
{
public:
    /// Constructor.
    /// \param destinationIP The IP address where the message(s) will be sent.
    UDPSender(const char* destinationIP, unsigned destinationPort = static_cast<unsigned>(Port::UDP));

    // Destructor.
    ~UDPSender();

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
