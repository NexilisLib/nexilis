#include <nexilis/af_inet/udp_sender.hh>
#include <nexilis/ports.hh>

#include <sys/socket.h>
#include <unistd.h>

#include <iostream>

namespace nexilis
{

UDPSender::UDPSender(const char* destinationIP) :
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

UDPSender::~UDPSender()
{
    close(m_socket);
}

void UDPSender::sendMessage(const std::string& message)
{
    std::cout << m_destinationAddress.sin_port << std::endl;

    ssize_t bytes_sent = sendto(m_socket, message.c_str(), message.size(), 0,
            (struct sockaddr*)&m_destinationAddress, sizeof(m_destinationAddress));

    if (bytes_sent == -1)
    {
        std::cerr << "Error sending message" << std::endl;
        close(m_socket);
    }
}

}
