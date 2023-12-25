#include "nexilis/af_inet/udp_server.hh"
#include <nexilis/dispatcher.hh>
#include <nexilis/protocol_manager.hh>
#include <nexilis/log.hh>

#include <sys/types.h>

void sendUdpMessage()
{
    const char* target_ip = "localhost";
    uint16_t target_port = 54200;

    int udp_socket = socket(AF_INET, SOCK_DGRAM, 0);

    if (udp_socket == -1)
    {
        std::cerr << "Error creating socket" << std::endl;
    }

    sockaddr_in target_addr;
    target_addr.sin_family = AF_INET;
    target_addr.sin_port = htons(target_port);
    target_addr.sin_addr.s_addr = inet_addr(target_ip);

    // TODO We want to change this message to be:
    // 2 bytes [0x10, 0x10]
    const char* message = "Test";

    ssize_t bytes_sent = sendto(udp_socket, message, strlen(message), 0, (sockaddr*)&target_addr, sizeof(target_addr));

    if (bytes_sent == -1)
    {
        std::cerr << "Error sending message" << std::endl;
        close(udp_socket);
    }

    close(udp_socket);
}

int main()
{
    nexilis::Log::startConsoleLogging();

    nexilis::ProtocolManager manager;

    auto server = manager.addProtocol<nexilis::UDPServer>(54209);

    std::thread serverThread([&server]()
    {
        server.start();
    });

    serverThread.join();

    // Add sending of the message here.
    sendUdpMessage();

    return 0;
}
