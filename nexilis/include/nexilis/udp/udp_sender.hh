#ifndef NEXILIS_UDP_SERVER_HH
#define NEXILIS_UDP_SERVER_HH

#include "../ports.hh"

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>


#include <cstdint>
#include <iostream>
#include <string>
#include <cstring>
#include <type_traits>

namespace nexilis
{

class UDPSender
{
public:
    UDPSender(const char* destinationIP) :
        m_destinationIP(destinationIP)
    {
        // Create a UDP socket
        m_socket = socket(AF_INET, SOCK_DGRAM, 0);
        if (m_socket == -1)
        {
            std::cerr << "Error creating socket" << std::endl;
        }

        m_destinationAddress.sin_family = AF_INET;

        m_destinationAddress.sin_port = htons(static_cast<uint16_t>(Port::UDP));
        m_destinationAddress.sin_addr.s_addr = inet_addr(m_destinationIP);
    }

    ~UDPSender()
    {
        close(m_socket);
    }

    void sendMessage(const std::string& message)
    {
        ssize_t bytes_sent = sendto(m_socket, message.c_str(), message.size(), 0,
                (struct sockaddr*)&m_destinationAddress, sizeof(m_destinationAddress));

        if (bytes_sent == -1)
        {
            std::cerr << "Error sending message" << std::endl;
            close(m_socket);
        }
    }

private:

    int m_socket;

    const char* m_destinationIP;
    sockaddr_in m_destinationAddress;
};

}

#endif
