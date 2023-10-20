#ifndef NEXILIS_UDP_SERVER_HH
#define NEXILIS_UDP_SERVER_HH

#include <arpa/inet.h>

#include <string>

namespace nexilis
{

class UDPSender
{
public:
    /// Constructor.
    /// \param destinationIP The IP address where the message(s) will be sent.
    UDPSender(const char* destinationIP);

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

}

#endif
