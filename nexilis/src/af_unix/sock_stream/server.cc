#include <nexilis/af_unix/sock_stream/server.hh>
#include <nexilis/nexilis_macros.hh>
#include <nexilis/command.hh>
#include <nexilis/server_manager.hh>

#include <sys/types.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>

namespace nexilis::af_unix::sock_stream
{

Server::Server(const std::string& socketPath) :
    Protocol(),
    m_socketPath(socketPath)
{
    m_buffer = new char[NEXILIS_BUFFER];
    createSocket();
    bindSocket();
}

Server::~Server()
{
    close(m_serverSocket);
    delete[] m_buffer;
}

Server::Server(Server&& other) :
    m_socketPath(std::move(other.m_socketPath)),
    m_serverSocket(std::move(other.m_serverSocket)),
    m_buffer(std::move(std::move(other.m_buffer)))
{
}

Server& Server::operator=(Server&& other)
{
    if (this != &other)
    {
        m_socketPath = std::move(other.m_socketPath);
        m_serverSocket = std::move(other.m_serverSocket);
        m_buffer = std::move(other.m_buffer);
    }
    return *this;
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
    if (unlink(m_socketPath.c_str()) != 0)
    {
        perror("unlink");
        std::cout << "Failed to unlink the socket file from " << m_socketPath << std::endl;
    }

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

void Server::receiveMessage()
{
    memset(m_buffer, '\0', NEXILIS_BUFFER);

    int clientSocket = accept(m_serverSocket, nullptr, nullptr);
    if (clientSocket == -1)
    {
        perror("accept");
        close(m_serverSocket);
    }

    ssize_t bytesRead = recv(clientSocket, m_buffer, sizeof(m_buffer), 0);

    if (bytesRead == -1)
    {
        perror("recv");
    }
    else
    {
        m_buffer[bytesRead] = '\0';
        std::cout << "Received message from client: " << m_buffer << std::endl;

        auto msg = getMessageHandler().readMessage("", std::string(m_buffer), -1, Command::getAuthentication());

        if (msg.getClient())
        {
            bool readCommand = Command::read(msg.getData(), *msg.getClient(), *this,
                    [this, &clientSocket](const std::vector<uint8_t>& message) { sendMessage(clientSocket, message); }
                    );

            if (readCommand)
            {
                std::cout << "Command read succesfully!" << std::endl;
            }
            else
            {
                std::cerr << "Server message reading error!" << std::endl;
            }
        }
        else
        {
            std::cerr << "Message from unauthorized client!" << std::endl;
        }
    }
}

}
