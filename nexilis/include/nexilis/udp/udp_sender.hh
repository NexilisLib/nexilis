#ifndef NEXILIS_UDP_SERVER_HH
#define NEXILIS_UDP_SERVER_HH

#include <iostream>
#include <string>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>

namespace nexilis
{

class UDPSender
{
public:
    UDPSender(const std::string& destinationIP, unsigned short destinationPort) :
        m_destinationIP(destinationIP),
        m_destinationPort(destinationPort)
    {
        // Create a UDP socket
        m_socket = socket(AF_INET, SOCK_DGRAM, 0);
        if (m_socket == -1)
        {
            std::cerr << "Error creating socket" << std::endl;
        }

        // Set up the server address structure
        std::memset(&m_serverAddress, 0, sizeof(m_serverAddress));
        m_serverAddress.sin_family = AF_INET;
        m_serverAddress.sin_port = htons(m_destinationPort);
        inet_pton(AF_INET, m_destinationIP.c_str(), &m_serverAddress.sin_addr);
    }

    ~UDPSender()
    {
        //close(m_socket);
    }

    void sendMessage(const std::string& message)
    {
        sendto(m_socket, message.c_str(), message.size(), 0,
               reinterpret_cast<struct sockaddr*>(&m_serverAddress), sizeof(m_serverAddress));
    }

private:
    int m_socket;
    std::string m_destinationIP;
    unsigned short m_destinationPort;
    struct sockaddr_in m_serverAddress;
};

}

#endif
