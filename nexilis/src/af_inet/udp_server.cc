#include <nexilis/af_inet/udp_server.hh>

#include <nexilis/command.hh>
#include <nexilis/connection_storage.hh>
#include <string>

namespace nexilis
{

UDPServer::UDPServer(unsigned port) :
    Protocol(port)
{
    struct addrinfo hints, *res, *p;

    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;

    if (getaddrinfo(nullptr, std::to_string(port).c_str(), &hints, &res) != 0)
    {
        Log::critical("Cannot get the address info");
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
        Log::critical("Failed to bind socket");
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(res);
}

UDPServer::~UDPServer()
{
    close(m_serverSocket);
}

void UDPServer::receiveMessage()
{
    char buffer[1024];
    struct sockaddr_storage clientAddr;
    socklen_t clientLen = sizeof(clientAddr);

    // Receive messages from the client.
    ssize_t bytesRead = recvfrom(m_serverSocket, buffer, sizeof(buffer), 0, (struct sockaddr*)&clientAddr, &clientLen);
    if (bytesRead == -1)
    {
        Log::critical("Receive failed");
        return;
    }

    // Hold either IPV4 or IPV6 address.
    char addressBuffer[INET6_ADDRSTRLEN];
    const char* address;

    // IPV4
    if (clientAddr.ss_family == AF_INET)
    {
        struct sockaddr_in* ipv4 = (struct sockaddr_in*)&clientAddr;
        address = inet_ntop(AF_INET, &(ipv4->sin_addr), addressBuffer, INET_ADDRSTRLEN);
    }

    // IPV6
    else if (clientAddr.ss_family == AF_INET6)
    {
        struct sockaddr_in6* ipv6 = (struct sockaddr_in6*)&clientAddr;
        address = inet_ntop(AF_INET6, &(ipv6->sin6_addr), addressBuffer, INET6_ADDRSTRLEN);
    }
    else
    {
        Log::critical("Unknown address family");
        return;
    }

    // Implicit conversion from const char* -> string?
    Connection connection(address);
    if (!ConnectionStorage::contains(connection))
    {
        ConnectionStorage::add(std::move(connection));
    }

    // Null-terminate the received message and convert it to std::string.
    buffer[bytesRead] = '\0';

    // Create message object.
    std::string message(buffer);

    if (!Command::read(message.c_str(), message.size(), connection))
    {
        Log::error("UDP server message reading error, message: ", message);
    }
}

} // namespace nexilis
