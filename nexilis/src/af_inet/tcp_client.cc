#include <nexilis/af_inet/tcp_client.hh>

#include <arpa/inet.h>
#include <unistd.h>

#include <cstring>

namespace nexilis::af_inet
{

TCPClient::TCPClient(ClientAPI& api) : 
    Protocol(api.getInetTCPPortNumber()),
    m_api(api)
{
    m_clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (m_clientSocket == -1)
    {
        Log::critical("TCPClient: Error creating socket");
    }

    memset(&m_serverAddr, 0, sizeof(m_serverAddr));
    m_serverAddr.sin_family = AF_INET;
    m_serverAddr.sin_port = htons(m_api.getInetTCPPortNumber());

    if (inet_pton(AF_INET, m_api.getInetTCPServerAddress().c_str(), &m_serverAddr.sin_addr) <= 0)
    {
        Log::critical("TCPClient: Invalid server address!");
    }
}

TCPClient::~TCPClient()
{
    close(m_clientSocket);
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
