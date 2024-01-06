#include <mutex>
#include <netinet/in.h>
#include <nexilis/af_inet/base_udp_server.hh>
#include <nexilis/log.hh>

#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

#include <cstring>

namespace nexilis
{

BaseUdpServer::BaseUdpServer(unsigned port) :
    Protocol(port)
{
    addrinfo hints, *res, *p;

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

BaseUdpServer::~BaseUdpServer()
{
    stop();
    close(m_serverSocket);
}

void BaseUdpServer::start()
{
    m_running = true;
    m_recvThread = std::thread(&BaseUdpServer::receiverThread, this);
}

void BaseUdpServer::stop()
{
    m_running = false;
    m_condition.notify_all();
    if (m_recvThread.joinable())
    {
        m_recvThread.join();
    }
}

bool BaseUdpServer::getNextMessage(Message& msg)
{
    std::lock_guard<std::mutex> lock(m_mtx);
    if (!m_messageQueue.empty())
    {
        msg = m_messageQueue.front();
        m_messageQueue.pop();
        return true;
    }
    return false;
}

void BaseUdpServer::receiverThread()
{
    while (m_running)
    {
        char buffer[1024];
        sockaddr_storage clientAddr;
        socklen_t clientLen = sizeof(clientAddr);
        ssize_t bytesRead = recvfrom(m_serverSocket, buffer, sizeof(buffer), 0, (sockaddr*)&clientAddr, &clientLen);

        if (bytesRead == -1)
        {
            Log::critical("Receive failed");
            continue;
        }

        // Hold either IPV4 or IPV6 address.
        char addressBuffer[INET6_ADDRSTRLEN];
        const char* address;

        // IPV4
        if (clientAddr.ss_family == AF_INET)
        {
            sockaddr_in* ipv4 = (sockaddr_in*)&clientAddr;
            address = inet_ntop(AF_INET, &(ipv4->sin_addr), addressBuffer, INET_ADDRSTRLEN);
        }

        // IPV6
        else if (clientAddr.ss_family == AF_INET6)
        {
            sockaddr_in6* ipv6 = (sockaddr_in6*)&clientAddr;
            address = inet_ntop(AF_INET6, &(ipv6->sin6_addr), addressBuffer, INET6_ADDRSTRLEN);
        }
        else
        {
            Log::critical("Unknown address family");
            address = "";
        }

        buffer[bytesRead] = '\0';
        std::string receivedData(buffer);

        Message msg
        {
            address,
            receivedData
        };

        {
            std::lock_guard<std::mutex> lock(m_mtx);
            m_messageQueue.push(msg);
        }

        // Notify waiting threads.
        m_condition.notify_one();
    }
}

}
