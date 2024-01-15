#ifndef NEXILIS_UDP_CLIENT_HH
#define NEXILIS_UDP_CLIENT_HH

#include <cstdint>
#include <netinet/in.h>
#include <nexilis/log.hh>
#include <nexilis/command.hh>
#include <nexilis/client_protocol.hh>
#include <nexilis/client_api/client_api.hh>
#include <nexilis/common/util.hh>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>

namespace nexilis::af_inet
{

class UDPClient : public Protocol, public ClientProtocol
{
public:
    UDPClient(ClientAPI& api) :
        Protocol(api.getServerAfInetUDPPortNumber()), m_api(api)
    {
        memset(&m_serverAddr, 0, sizeof(m_serverAddr));
        m_serverAddr.sin_family = AF_INET;
        m_serverAddr.sin_port = htons(api.getServerAfInetUDPPortNumber());
        if (inet_pton(AF_INET, api.getServerAddress().c_str(), &m_serverAddr.sin_addr) <= 0)
        {
            std::cerr << "Invalid server address" << std::endl;
            exit(EXIT_FAILURE);
        }

        m_clientSocket = createSocket();
    }

    int createSocket()
    {
        int socketFD = socket(AF_INET, SOCK_DGRAM, 0);
        if (socketFD == -1)
        {
            std::cerr << "Failed to create socket." << std::endl;
            exit(EXIT_FAILURE);
        }
        return socketFD;
    }

    // Send data using UDP
    void sendData(const char* data, size_t dataSize)
    {
        auto serverAddr = (const struct sockaddr*)&m_serverAddr;
        sendto(m_clientSocket, data, dataSize, 0, serverAddr, sizeof(m_serverAddr));
    }

    void receiveData(char* buffer, size_t bufferSize, struct sockaddr* srcAddr, socklen_t* srcAddrLen)
    {
        recvfrom(m_clientSocket, buffer, bufferSize, 0, srcAddr, srcAddrLen);
    }

    // Receive data using UDP
    /*
    void receiveData(char* buffer, size_t bufferSize)
    {
        auto serverAddr = (struct sockaddr*)&m_serverAddr;
        socklen_t serverAddrLen = sizeof(m_serverAddr);
        recvfrom(m_clientSocket, buffer, bufferSize, 0, serverAddr, &serverAddrLen);
    }
    */

    void attach()
    {
        m_receiverThread = std::thread(&UDPClient::receiveLoop, this);
    }

    void start() override
    {
    }

    /// Protocol::stop() implementation.
    void stop() override
    {
        // Join the thread when stopping
        if (m_receiverThread.joinable())
        {
            m_receiverThread.join();
        }
    }

    /// Protocol::getType() implementation.
    Type getType() override
    {
        return Type::UDP;
    }

private:
    void receiveLoop()
    {
        while (true)
        {
            char buffer[1024];
            struct sockaddr srcAddr;
            socklen_t srcAddrLen;

            receiveData(buffer, sizeof(buffer), &srcAddr, &srcAddrLen);
            std::cout << "data" << buffer << std::endl;

            auto message = Util::convertToByteVector(buffer, sizeof(buffer));

            m_api.readMessage(message);
        }
    }

private:
    int m_clientSocket;
    struct sockaddr_in m_serverAddr;

    std::thread m_receiverThread;

    ClientAPI& m_api;
};

} // namespace nexilis::af_inet

#endif
