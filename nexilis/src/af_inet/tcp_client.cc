#include <nexilis/af_inet/tcp_client.hh>
#include <nexilis/log.hh>

#include <arpa/inet.h>
#include <unistd.h>

#include <cstring>

namespace nexilis::af_inet
{

TCPClient::TCPClient(ClientAPI& api) : 
    ClientProtocol(&api)
{
    m_clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (m_clientSocket == -1)
    {
        Log::critical("TCPClient: Error creating socket");
    }

    memset(&m_serverAddr, 0, sizeof(m_serverAddr));
    m_serverAddr.sin_family = AF_INET;
    m_serverAddr.sin_port = htons(ClientProtocol::getClientAPI()->getInetTCPPortNumber());

    if (inet_pton(AF_INET, ClientProtocol::getClientAPI()->getInetTCPServerAddress().c_str(), &m_serverAddr.sin_addr) <= 0)
    {
        Log::critical("TCPClient: Invalid server address!");
    }
}

TCPClient::~TCPClient()
{
    close(m_clientSocket);
}

TCPClient::TCPClient(TCPClient&& other) :
    Protocol(std::move(other)),
    ClientProtocol(std::move(other)),
    m_clientSocket(std::move(other.m_clientSocket)),
    m_serverAddr(std::move(other.m_serverAddr))
{
}

TCPClient& TCPClient::operator=(TCPClient&& other)
{
    if (this != &other)
    {
        Protocol::operator=(std::move(other));
        ClientProtocol::operator=(std::move(other));
        m_clientSocket = std::move(other.m_clientSocket);
        m_serverAddr = std::move(other.m_serverAddr);
    }
    return *this;
}

void TCPClient::start()
{
}

void TCPClient::sendMessage(const std::string& message)
{
    bool sentMessage = send(message.c_str(), message.size());

    if (!sentMessage)
    {
        Log::error("TCPClient: Error sending message");
    }
}

void TCPClient::sendMessage(const std::vector<uint8_t>& message)
{
    const char* data = reinterpret_cast<const char*>(message.data());
    bool sentMessage = send(data, message.size());

    if (!sentMessage)
    {
        Log::error("TCPClient: Error sending message");
    }
}

bool TCPClient::connectToServer()
{
    return connect(m_clientSocket, (sockaddr*)&m_serverAddr, sizeof(m_serverAddr)) == 0;
}

bool TCPClient::send(const char* data, size_t dataSize)
{
    return write(m_clientSocket, data, dataSize) == static_cast<long>(dataSize);
}

bool TCPClient::receive(char* buffer, size_t bufferSize)
{
    ssize_t bytesRead = read(m_clientSocket, buffer, bufferSize);
    return bytesRead > 0;
}

}
