#include <nexilis/af_unix/sock_stream/client.hh>
#include <nexilis/nexilis_macros.hh>
#include <nexilis/log.hh>

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

namespace nexilis::af_unix::sock_stream
{

Client::Client(ClientAPI& clientApi) :
    ClientProtocol(&clientApi),
    m_serverSocketPath(clientApi.getUnixStreamPath()),
    m_mutex(std::make_unique<std::mutex>())
{
    createSocket();
    connectToServer();
}

Client::~Client()
{
    if (m_clientSocket == -1)
    {
        close(m_clientSocket);
    }

    if (m_receiveThread.joinable())
    {
        m_receiveThread.join();
    }
}

Client::Client(Client&& other) :
    Protocol(std::move(other)),
    ClientProtocol(std::move(other)),
    m_serverSocketPath(std::move(other.m_serverSocketPath)),
    m_clientSocket(std::move(other.m_clientSocket)),
    m_serverAddr(std::move(other.m_serverAddr)),
    m_receiveThread(std::move(other.m_receiveThread)),
    m_mutex(std::move(other.m_mutex))
{
}

Client& Client::operator=(Client&& other)
{
    if (this != &other)
    {
        Protocol::operator=(std::move(other));
        ClientProtocol::operator=(std::move(other));
        m_serverSocketPath = std::move(other.m_serverSocketPath);
        m_clientSocket = std::move(other.m_clientSocket);
        m_serverAddr = std::move(other.m_serverAddr);
        m_receiveThread = std::move(other.m_receiveThread);
        m_mutex = std::move(other.m_mutex);
    }
    return *this;
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
    std::lock_guard<std::mutex> lock(*m_mutex);
    m_serverAddr.sun_family = AF_UNIX;
    strcpy(m_serverAddr.sun_path, m_serverSocketPath.c_str());

    if (connect(m_clientSocket, (struct sockaddr*)&m_serverAddr, sizeof(m_serverAddr)) == -1)
    {
        perror("connect");
        close(m_clientSocket);
        m_clientSocket = -1;
    }
}

void Client::sendMessage(const std::string& message)
{
    Log::debug("af_unix::sock_stream::Client: Sending message to : ", m_serverAddr.sun_path);

    std::string realMsg = message + '\0';

    ssize_t sentBytes = send(m_clientSocket, realMsg.c_str(), realMsg.size(), 0);

    if (sentBytes == -1)
    {
        Log::error("af_unix::sock_stream::Client: Error sending message");
        perror("send");
    }
}

void Client::sendMessage(const std::vector<uint8_t>& message)
{
    Log::debug("af_unix::sock_stream::Client: Sending message to : ", m_serverAddr.sun_path);

    std::vector<uint8_t> realMsg = message;
    realMsg.push_back('\0');
    
    ssize_t sentBytes = send(m_clientSocket, realMsg.data(), realMsg.size(), 0);

    if (sentBytes == -1)
    {
        Log::error("af_unix::sock_stream::Client: Error sending message");
        perror("send");
    }
}

void Client::start()
{
    m_receiveThread = std::thread([this]()
    {
        while (true)
        {
            auto data = receiveMessage();
            ClientProtocol::getClientAPI()->readMessage(data);
        }
    });
}

void Client::stop()
{
    close(m_clientSocket);
}

std::vector<uint8_t> Client::receiveMessage()
{
    std::lock_guard<std::mutex> lock(*m_mutex);
    // Receive buffer.
    std::vector<uint8_t> receivedData(NEXILIS_BUFFER);

    // Receive data into buffer.
    ssize_t bytesRead = recv(m_clientSocket, receivedData.data(), receivedData.size(), 0);

    if (bytesRead == -1)
    {
        perror("recv");
        close(m_clientSocket);
        m_clientSocket = -1;
    }

    receivedData.resize(bytesRead);
    return receivedData;
}

}
