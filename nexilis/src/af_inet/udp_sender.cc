#include <nexilis/af_inet/udp_sender.hh>
#include <nexilis/ports.hh>
#include <nexilis/log.hh>

#include <sys/socket.h>
#include <unistd.h>

namespace nexilis
{

UDPSender::UDPSender(const char* destinationIP, unsigned port) :
    m_destinationIP(destinationIP)
{
    // Create a UDP socket
    m_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (m_socket == -1)
    {
        Log::critical("Error creating socket!");
    }

    m_destinationAddress.sin_family = AF_INET;
    m_destinationAddress.sin_port = htons(port);
    m_destinationAddress.sin_addr.s_addr = inet_addr(m_destinationIP);
}

UDPSender::~UDPSender()
{
    close(m_socket);
}

void UDPSender::sendMessage(const std::string& message)
{
    Log::info("Sending message: ", message);

    ssize_t bytes_sent = sendto(m_socket, message.c_str(), message.size(), 0,
            (struct sockaddr*)&m_destinationAddress, sizeof(m_destinationAddress));

    if (bytes_sent == -1)
    {
        Log::critical("Error sending message!");
        close(m_socket);
    }
}

}
