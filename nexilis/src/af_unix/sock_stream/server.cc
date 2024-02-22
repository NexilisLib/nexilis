#include <nexilis/af_unix/sock_stream/server.hh>
#include <nexilis/nexilis_macros.hh>
#include <nexilis/command.hh>
#include <nexilis/server_manager.hh>

#include <sys/types.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <unistd.h>

namespace nexilis::af_unix::sock_stream
{

Server::Server(const std::string& socketPath) :
    m_socketPath(socketPath),
    m_buffer(NEXILIS_BUFFER)
{
    createSocket();
    bindSocket();
}

Server::~Server()
{
    if (m_receiveThread.joinable())
    {
        m_receiveThread.join();
    }

    close(m_serverSocket);
}

Server::Server(Server&& other) :
    m_socketPath(std::move(other.m_socketPath)),
    m_serverSocket(std::move(other.m_serverSocket)),
    m_buffer(std::move(std::move(other.m_buffer))),
    m_receiveThread(std::move(other.m_receiveThread))
{
}

Server& Server::operator=(Server&& other)
{
    if (this != &other)
    {
        m_socketPath = std::move(other.m_socketPath);
        m_serverSocket = std::move(other.m_serverSocket);
        m_buffer = std::move(other.m_buffer);
        m_receiveThread = std::move(other.m_receiveThread);
    }
    return *this;
}

void Server::start()
{
    m_receiveThread = std::thread([this]()
    {
        while (true)
        {
            handleMessages();
        }
    });
}

void Server::createSocket()
{
    m_serverSocket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (m_serverSocket == -1)
    {
        perror("socket");
    }
}

void Server::bindSocket()
{
    sockaddr_un serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sun_family = AF_UNIX;
    strcpy(serverAddr.sun_path, m_socketPath.c_str());

    // Remove old socket file.
    // This operation will fail if this is the first usage and it's okay.
    unlink(m_socketPath.c_str());

    if (bind(m_serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
    {
        perror("bind");
        close(m_serverSocket);
    }

    if (listen(m_serverSocket, ServerManager::getMaxAmountOfClients()) == -1)
    {
        perror("listen");
        close(m_serverSocket);
    }
}

void Server::sendMessage(int clientSocket, const std::vector<uint8_t>& message)
{
    ssize_t sentBytes = send(clientSocket, message.data(), sizeof(message), 0);

    if (sentBytes == -1)
    {
        perror("send");
    }
}

void Server::handleMessages()
{
    memset(m_buffer.data(), '\0', m_buffer.size());

    int clientSocket = accept(m_serverSocket, nullptr, nullptr);
    if (clientSocket == -1)
    {
        perror("accept");
        close(m_serverSocket);
    }

    ssize_t bytesRead = recv(clientSocket, m_buffer.data(), m_buffer.size(), 0);

    if (bytesRead == -1)
    {
        perror("recv");
    }
    else
    {
        std::string strMsg = std::string(m_buffer.begin(), m_buffer.end());
        strMsg.resize(bytesRead);
        auto msg = getMessageHandler().readMessage("localhost", strMsg, -1, Command::getAuthentication());

        if (msg.getClient())
        {
            bool readCommand = Command::read(msg.getData(), *msg.getClient(), *this,
                    [this, &clientSocket](const std::vector<uint8_t>& message) 
                    { sendMessage(clientSocket, message); }
            );

            if (readCommand)
            {
                Log::debug("af_unix::sock_stream::Server: Command read succesfully!");
            }
            else
            {
                Log::error("af_unix::sock_stream::Server: Message reading error!");
            }
        }
        else
        {
            Log::error("af_unix::sock_stream::Server: Message from unauthorized client!");
        }
    }
}

}
