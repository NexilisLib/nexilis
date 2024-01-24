#include <cstdint>
#include <nexilis/af_unix/sock_stream/client.hh>
#include <nexilis/buffer.hh>

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

namespace nexilis::af_unix::sock_stream
{

Client::Client(ClientAPI& clientApi) :
    Protocol(),
    m_api(clientApi),
    m_serverSocketPath(m_api.getUnixSocketServerPath())
{
    createSocket();
    connectToServer();

    std::cout << "Sending password to server: " << m_api.getClientPassword() << std::endl;
    std::cout << "Server socket path: " << m_api.getUnixSocketServerPath() << std::endl;

    sendMessage(m_api.getClientPassword());
}

Client::~Client()
{
    if (m_clientSocket == -1)
    {
        close(m_clientSocket);
    }
}

void Client::createSocket()
{
    m_clientSocket = socket(AF_UNIX, SOCK_STREAM, 0);

    if (m_clientSocket == -1)
    {
        perror("socket");
        close(m_clientSocket);
    }
}

void Client::connectToServer()
{
    m_serverAddr.sun_family = AF_UNIX;
    strcpy(m_serverAddr.sun_path, m_serverSocketPath.c_str());

    if (connect(m_clientSocket, (struct sockaddr*)&m_serverAddr, sizeof(m_serverAddr)) == -1)
    {
        perror("connect");
        close(m_clientSocket);
    }
}

void Client::sendMessage(const std::string& message)
{
    std::cout << "Sending message to : " << m_serverAddr.sun_path << std::endl;

    ssize_t sentBytes = send(m_clientSocket, message.c_str(), message.size(), 0);

    if (sentBytes == -1)
    {
        perror("send");
    }
}

void Client::start()
{
    while (true)
    {
        auto data = receiveMessage();
        m_api.readMessage(data);
    }
}

void Client::stop()
{
    close(m_clientSocket);
}

std::vector<uint8_t> Client::receiveMessage()
{
    // Receive buffer.
    std::vector<uint8_t> receivedData(NEXILIS_BUFFER);

    // Receive data into buffer.
    ssize_t bytesRead = recv(m_clientSocket, receivedData.data(), receivedData.size(), 0);

    if (bytesRead == -1)
    {
        perror("recv");
        close(m_clientSocket);
    }

    receivedData.resize(bytesRead);
    return receivedData;
}

}
