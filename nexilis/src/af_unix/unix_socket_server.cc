#include <nexilis/af_unix/unix_socket_server.hh>
#include <nexilis/command.hh>
#include <nexilis/client_storage.hh>

#include <arpa/inet.h>
#include <sys/un.h>

#include <csignal>

namespace nexilis::af_unix
{

/// The file path we are reading messages from.
static std::string path;

UnixSocketServer::UnixSocketServer(const std::string& socketPath)
    : Protocol(-1)
{
    path = socketPath;

    // TODO
    // This project most definately needs a global max buffer size for a message,
    // regardless of the protocol we are using.
    m_bufferSize = 1024;
    m_buffer = new char[m_bufferSize];
    createSocket();
    bindSocket();

    std::signal(SIGINT, signalHandler);
}

/// Destructor.
UnixSocketServer::~UnixSocketServer()
{
    close(m_serverSocket);
    delete[] m_buffer;
}

// Read messages.
void UnixSocketServer::receiveMessage()
{
    struct sockaddr_in clientAddress;
    socklen_t clientAddressLen = sizeof(clientAddress);
    memset(m_buffer, '\0', m_bufferSize);
    ssize_t bytesRead = recvfrom(m_serverSocket, m_buffer, m_bufferSize, 0, (struct sockaddr*)&clientAddress, &clientAddressLen);

    if (bytesRead > 0)
    {
        std::string address = std::string(inet_ntoa(clientAddress.sin_addr));
        auto message = m_messageHandler.readMessage(address, m_buffer, -1);

        if (message.client)
        {
            if (!Command::read(message.message, *message.client, *this))
            {
                Log::error("Unix socket server message reading error from message: ");
            }
        }
        else
        {
            Log::info("Message from unauhorized client!");
        }
    }
}

void UnixSocketServer::createSocket()
{
    m_serverSocket = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (m_serverSocket == -1)
    {
        Log::critical("Error creating socket");
    }
}

void UnixSocketServer::bindSocket()
{
    struct sockaddr_un serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sun_family = AF_UNIX;
    strncpy(serverAddr.sun_path, path.c_str(), sizeof(serverAddr.sun_path) - 1);

    if (bind(m_serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
    {
        Log::critical("Error binding socket");
        close(m_serverSocket);
        exit(1);
    }
}

void UnixSocketServer::signalHandler(int signum)
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

} // namespace nexilis::af_unix
