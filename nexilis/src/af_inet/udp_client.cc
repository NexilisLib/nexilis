#include <nexilis/af_inet/udp_client.hh>

#include <cstring>
#include <arpa/inet.h>

namespace nexilis::af_inet
{

UDPClient::UDPClient(ClientAPI& api) :
    Protocol(api.getServerAfInetUDPPortNumber()), m_api(api)
{
    memset(&m_serverAddr, 0, sizeof(m_serverAddr));
    m_serverAddr.sin_family = AF_INET;
    m_serverAddr.sin_port = htons(api.getServerAfInetUDPPortNumber());
    if (inet_pton(AF_INET, api.getServerAddress().c_str(), &m_serverAddr.sin_addr) <= 0)
    {
        Log::critical("Invalid server address");
        exit(EXIT_FAILURE);
    }

    m_clientSocket = createSocket();

    // I don't actually know if this should be abstracted or not.
    sendMessage(m_api.getClientPassword());
}

int UDPClient::createSocket()
{
    int socketFD = socket(AF_INET, SOCK_DGRAM, 0);
    if (socketFD == -1)
    {
        Log::critical("Failed to create socket.");
        exit(EXIT_FAILURE);
    }
    return socketFD;
}

void UDPClient::sendMessage(const std::string& message)
{
    sendData(message.c_str(), message.size());
}

// Send data using UDP
void UDPClient::sendData(const char* data, size_t dataSize)
{
    auto serverAddr = (const struct sockaddr*)&m_serverAddr;
    sendto(m_clientSocket, data, dataSize, 0, serverAddr, sizeof(m_serverAddr));
}

void UDPClient::receiveData(char* buffer, size_t bufferSize, struct sockaddr* srcAddr, socklen_t* srcAddrLen)
{
    recvfrom(m_clientSocket, buffer, bufferSize, 0, srcAddr, srcAddrLen);
}

void UDPClient::start()
{
    m_receiverThread = std::thread(&UDPClient::receiveLoop, this);
}

/// Protocol::stop() implementation.
void UDPClient::stop()
{
    // Join the thread when stopping
    if (m_receiverThread.joinable())
    {
        m_receiverThread.join();
    }
}

void UDPClient::receiveLoop()
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

}
