#include <nexilis/af_unix/sock_dgram/server.hh>
#include <nexilis/command.hh>
#include <nexilis/client_storage.hh>
#include <nexilis/nexilis_macros.hh>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>

#include <unistd.h>

#include <csignal>

namespace nexilis::af_unix::sock_dgram
{

/// The file path we are reading messages from.
static std::string path;

Server::Server(const std::string& socketPath) :
    m_buffer(NEXILIS_BUFFER)
{
    path = socketPath;

    createSocket();
    bindSocket();

    std::signal(SIGINT, signalHandler);
}

Server::Server(Server&& other) :
    Protocol(std::move(other)),
    ServerProtocol(std::move(other)),
    m_serverSocket(other.m_serverSocket),
    m_buffer(other.m_buffer)
{
}

Server& Server::operator=(Server&& other)
{
    if (this != &other)
    {
        Protocol::operator=(std::move(other));
        ServerProtocol::operator=(std::move(other));
        m_serverSocket = std::move(other.m_serverSocket);
        m_buffer = std::move(other.m_buffer);
    }
    return *this;
}

Server::~Server()
{
    close(m_serverSocket);
}

// Read messages.
void Server::receiveMessage()
{
    struct sockaddr_un clientAddress;
    socklen_t clientAddressLen = sizeof(clientAddress);

    memset(&clientAddress, 0, sizeof(clientAddress));
    memset(m_buffer.data(), '\0', m_buffer.size());
    clientAddress.sun_family = AF_UNIX;
    
    // Server is sending messages to itself with this.
    //strcpy(clientAddress.sun_path, "/tmp/nexilis");

    ssize_t bytesRead = recvfrom(m_serverSocket, m_buffer.data(), m_buffer.size(), 0, (struct sockaddr*)&clientAddress, &clientAddressLen);

    if (bytesRead > 0)
    {
        std::cout << "Received data: " << m_buffer.data() << std::endl;

        std::cout << "Received address family: " << clientAddress.sun_family << std::endl;
        std::cout << "Received address path: " << clientAddress.sun_path << std::endl;

        if (clientAddress.sun_family == AF_UNIX)
        {
            std::string testMessage = "client";

            std::cout << "CLIENT ADDRESS PATH: " << clientAddress.sun_path << std::endl;
            std::cout << "CLIENT ADDRESS LEN: " << clientAddressLen << std::endl;

            ssize_t reply_send = sendto(m_serverSocket, testMessage.c_str(), testMessage.size(), 0, (struct sockaddr*)&clientAddress, sizeof(clientAddress));

            if (reply_send == -1)
            {
                std::cout << "Something went wrong with the send of the reply" << std::endl;
                std::cout << "REPLY SEND DATA: " << reply_send << std::endl;
                std::cout << "CLIENT ADDRESS PATH: " << clientAddress.sun_path << std::endl;
                std::cout << "CLIENT ADDRESS FAMILY: " << clientAddress.sun_family << std::endl;
                std::cout << "CLIENT ADDRESS LEN: " << clientAddressLen << std::endl;
                perror("sendto");
            }
            else
            {
                std::cout << "Send reply to client" << std::endl;
            }


            //std::string address = std::string(inet_ntoa(clientAddress.sin_addr));
            auto message = getMessageHandler().readMessage("test", m_buffer.data(), -1, Command::getAuthentication());

            if (message.getClient())
            {
                // TODO
                if (!Command::read(message.getData(), *message.getClient(), *this, [](const std::vector<uint8_t>&){}))
                {
                    Log::error("Unix socket server message reading error from message: ");
                }
            }
            else
            {
                Log::info("UNIX: Message from unauhorized client!");
            }
        }
        else
        {
            std::cerr << "Received message from unexpected address family: " << clientAddress.sun_family << std::endl;
        }
    }
    else if (bytesRead == -1)
    {
        perror("recvfrom");
    }
}

void Server::createSocket()
{
    m_serverSocket = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (m_serverSocket == -1)
    {
        Log::critical("Error creating socket, reason: ", strerror(errno));
    }
}

void Server::bindSocket()
{
    struct sockaddr_un serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sun_family = AF_UNIX;
    strcpy(serverAddr.sun_path, path.c_str());

    // Remove old socket file. This operation will fail for first time usage.
    unlink(path.c_str());

    if (bind(m_serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
    {
        Log::critical("Error binding socket, reason: ", strerror(errno));
        close(m_serverSocket);
        exit(1);
    }
}

void Server::signalHandler(int signum)
{
    if (signum == SIGINT)
    {
        // Delete the file before exiting
        if (std::remove(path.c_str()) == 0)
        {
            Log::info("File deleted successfully: ", path);
        }
        else
        {
            Log::error("Error deleting file");
        }
        std::exit(signum);
    }
}

} // namespace nexilis::af_unix::sock_dgram
