#include <nexilis/client/protocol/af_unix/stream_client.hh>
#include <nexilis/logger/log.hh>
#include <nexilis/nexilis_constants.hh>

#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

namespace nexilis::client::af_unix
{

StreamClient::StreamClient(ClientAPI& clientApi)
    : ClientProtocol(&clientApi),
      m_serverSocketPath(clientApi.getUnixStreamPath()),
      m_mutex(std::make_unique<std::mutex>())
{
    createSocket();
    connectToServer();
}

StreamClient::~StreamClient()
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

StreamClient::StreamClient(StreamClient&& other)
    : Protocol(std::move(other)),
      ClientProtocol(std::move(other)),
      m_serverSocketPath(std::move(other.m_serverSocketPath)),
      m_clientSocket(std::move(other.m_clientSocket)),
      m_serverAddr(std::move(other.m_serverAddr)),
      m_receiveThread(std::move(other.m_receiveThread)),
      m_mutex(std::move(other.m_mutex))
{
}

StreamClient& StreamClient::operator=(StreamClient&& other)
{
    if (this != &other)
    {
        m_serverSocketPath = std::move(other.m_serverSocketPath);
        m_clientSocket = std::move(other.m_clientSocket);
        m_serverAddr = std::move(other.m_serverAddr);
        m_receiveThread = std::move(other.m_receiveThread);
        m_mutex = std::move(other.m_mutex);

        Protocol::operator=(std::move(other));
        ClientProtocol::operator=(std::move(other));
    }
    return *this;
}

void StreamClient::createSocket()
{
    m_clientSocket = socket(AF_UNIX, SOCK_STREAM, 0);

    if (m_clientSocket == -1)
    {
        perror("socket");
        close(m_clientSocket);
    }
}

void StreamClient::connectToServer()
{
    std::lock_guard<std::mutex> lock(*m_mutex);
    m_serverAddr.sun_family = AF_UNIX;
    strcpy(m_serverAddr.sun_path, m_serverSocketPath.c_str());

    auto address = reinterpret_cast<sockaddr*>(&m_serverAddr);
    if (connect(m_clientSocket, address, sizeof(m_serverAddr)) == -1)
    {
        perror("connect");
        close(m_clientSocket);
        m_clientSocket = -1;
    }
}

void StreamClient::sendMsg(const std::string& message)
{
    Log::debug("Sending message to: ", m_serverAddr.sun_path);

    std::string readyMessage = message + '\0';
    ssize_t sentBytes = send(m_clientSocket, readyMessage.c_str(), readyMessage.size(), 0);

    if (sentBytes == -1)
    {
        Log::error("Error sending message");
    }
}

void StreamClient::sendMessage(const nx_data& message)
{
    sendMsg(Util::convertToString(message));
}

void StreamClient::start()
{
    m_receiveThread = std::thread([this]()
                                  {
        while (true)
        {
            auto data = receiveMessage();
            ClientProtocol::getClientAPI()->readMessage(data);
        } });
    ClientProtocol::start(getType());
}

void StreamClient::stop()
{
    close(m_clientSocket);
}

nx_data StreamClient::receiveMessage()
{
    std::lock_guard<std::mutex> lock(*m_mutex);
    // Receive buffer.
    nx_data receivedData(NEXILIS_BUFFER);

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

} // namespace nexilis::client::af_unix
