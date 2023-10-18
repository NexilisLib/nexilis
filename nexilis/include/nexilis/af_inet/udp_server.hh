#ifndef NEXILIS_UDP_SERVER_HH
#define NEXILIS_UDP_SERVER_HH

#include <nexilis/ports.hh>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>

#include <iostream>
#include <cstdlib>
#include <cstring>

namespace nexilis
{

class UDPServer
{
public:
    UDPServer()
    {
        struct addrinfo hints, *res, *p;

        std::memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_DGRAM;
        hints.ai_flags = AI_PASSIVE;

        if (getaddrinfo(nullptr, portToString(Port::UDP), &hints, &res) != 0)
        {
            std::cerr << "Cannot get the address info" << std::endl;
            exit(EXIT_FAILURE);
        }

        // Loop through all the results and bind to the first suitable one.
        for (p = res; p != nullptr; p = p->ai_next)
        {
            m_serverSocket = socket(p->ai_family, p->ai_socktype, p->ai_protocol);

            // Fail.
            if (m_serverSocket == -1)
            {
                continue;
            }

            // Success.
            if (bind(m_serverSocket, p->ai_addr, p->ai_addrlen) == 0)
            {
                break;
            }
        }

        if (!p)
        {
            std::cerr << "failed to bind socket" << std::endl;
            exit(EXIT_FAILURE);
        }

        freeaddrinfo(res);
    }

    ~UDPServer()
    {
        close(m_serverSocket);
    }

    void receiveMessage()
    {
        char buffer[1024];
        struct sockaddr_storage clientAddr;
        socklen_t clientLen = sizeof(clientAddr);

        // Receive messages from the client.
        ssize_t bytesRead = recvfrom(m_serverSocket, buffer, sizeof(buffer), 0, (struct sockaddr *)&clientAddr, &clientLen);
        if (bytesRead == -1)
        {
            std::cerr << "Receive failed" << std::endl;
            return;
        }

        // This part needs to be refactored.
        // Null-terminate the received message and convert it to std::string.
        buffer[bytesRead] = '\0';
        std::string message(buffer);

        std::cout << "Received message: " << message << std::endl;
    }

private:
    int m_serverSocket;
};

}

#endif
