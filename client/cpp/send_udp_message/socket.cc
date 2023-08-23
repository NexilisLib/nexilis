#include <iostream>
#include <cstring>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

int main()
{
    const char* target_ip = "192.168.1.17";
    int target_port = 54200;

    int udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (udp_socket == -1)
    {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }

    sockaddr_in target_addr;
    target_addr.sin_family = AF_INET;
    target_addr.sin_port = htons(target_port);
    target_addr.sin_addr.s_addr = inet_addr(target_ip);

    const char* message = "Unix sockets say hey!";
    ssize_t bytes_sent = sendto(udp_socket, message, strlen(message), 0, (struct sockaddr*)&target_addr, sizeof(target_addr));
    if (bytes_sent == -1)
    {
        std::cerr << "Error sending message" << std::endl;
        close(udp_socket);
        return 1;
    }

    close(udp_socket);
    return 0;
}
