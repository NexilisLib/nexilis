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
    UDPSender(const char* destinationIP, unsigned port = static_cast<unsigned>(Port::UDP));

    // Destructor.
    ~UDPSender();

    /// Send message to to destination.
    /// \param message The message to be sent.
    void sendMessage(const std::string& message);

private:
    int m_socket;
    const char* m_destinationIP;
    sockaddr_in m_destinationAddress;
};

} // namespace nexilis

#endif
