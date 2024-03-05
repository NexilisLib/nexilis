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
    Loggable(typeToString(getType())),
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
    Protocol(std::move(other)),
    ServerProtocol(std::move(other)),
    Loggable(std::move(other)),
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
        Protocol::operator=(std::move(other));
        ServerProtocol::operator=(std::move(other));
        Loggable::operator=(std::move(other));
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
        Log::error(logName(), "Couldn't create socket");
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
        Log::error(logName(), "Failed to bind socket");
        close(m_serverSocket);
    }

    if (listen(m_serverSocket, ServerManager::getMaxAmountOfClients()) == -1)
    {
        Log::error(logName(), "Failed to listen to socket");
        close(m_serverSocket);
    }
}

void Server::sendMessage(int clientSocket, const std::vector<uint8_t>& message)
{
    ssize_t sentBytes = send(clientSocket, message.data(), sizeof(message), 0);

    if (sentBytes == -1)
    {
        Log::error(logName(), "Failed to send message");
    }
}

std::string Server::receiveMessage(int socket)
{
    std::string message;
    char buffer[NEXILIS_BUFFER];
    ssize_t bytesRead;

    while (true)
    {
        bytesRead = recv(socket, buffer, sizeof(buffer), 0);

        if (bytesRead > 0)
        {
            message.append(buffer, bytesRead);

            // Check if the message contains the null terminator.
            size_t nullPos = message.find('\0');

            if (nullPos != std::string::npos)
            {
                return message.substr(0, nullPos);
            }
            else
            {
                Log::error(logName(), "Received message that does" ,
                " not contain the null-termination character");
                break;
            }
        }

        else if (bytesRead == 0)
        {
            Log::info(logName(), "Connection closed by peer");
            break;
        }
        else
        {
            Log::error(logName(), "Error receiving message");
            break;
        }
    }
    return "";
}

void Server::handleMessages()
{
    memset(m_buffer.data(), '\0', m_buffer.size());

    int clientSocket = accept(m_serverSocket, nullptr, nullptr);
    if (clientSocket == -1)
    {
        Log::error(logName(), "Failed to accept connection");
        close(m_serverSocket);
    }

    while (true)
    {
        std::string message = receiveMessage(clientSocket);

        if (message == "")
        {
            break;
        }
        else
        {
            auto msg = getMessageHandler().readMessage("localhost", message, -1, Command::getAuthentication());

            if (msg.getClient())
            {
                bool readCommand = Command::read(msg.getData(), *msg.getClient(), *this,
                        [this, &clientSocket](const std::vector<uint8_t>& message) 
                        { sendMessage(clientSocket, message); }
                );

                if (readCommand)
                {
                    Log::debug(logName(), "Command read succesfully!");
                }
                else
                {
                    Log::error(logName(), "Message reading error!");
                }
            }
            else
            {
                Log::error(logName(), "Message from unauthorized client!");
            }
        }
    }
}

}
