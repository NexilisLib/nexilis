#include <nexilis/af_inet/udp_client.hh>
#include <nexilis/nexilis_macros.hh>

#include <arpa/inet.h>

namespace nexilis::af_inet
{

UDPClient::UDPClient(ClientAPI& api) :
    Protocol(api.getInetUDPPortNumber()), m_api(api)
{
    memset(&m_serverAddr, 0, sizeof(m_serverAddr));
    m_serverAddr.sin_family = AF_INET;
    m_serverAddr.sin_port = htons(api.getInetUDPPortNumber());
    if (inet_pton(AF_INET, api.getInetUDPServerAddress().c_str(), &m_serverAddr.sin_addr) <= 0)
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

std::vector<uint8_t> UDPClient::receiveData(sockaddr* srcAddr, socklen_t* srcAddrLen)
{
    std::vector<uint8_t> receivedData(NEXILIS_BUFFER);
    ssize_t bytesRead = recvfrom(m_clientSocket, receivedData.data(), receivedData.size(), 0, srcAddr, srcAddrLen);

    if (bytesRead == -1)
    {
        perror("recvfrom");
        Log::error("UDPClient receiveData");
    }

    receivedData.resize(bytesRead);
    return receivedData;
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
        sockaddr srcAddr;
        socklen_t srcAddrLen;

        auto data = receiveData(&srcAddr, &srcAddrLen);
        m_api.readMessage(data);
    }
}

}
