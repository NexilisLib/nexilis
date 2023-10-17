#include <nexilis/af_unix/unix_socket_server.hh>

#include <arpa/inet.h>
#include <sys/un.h>

#include <iostream>
#include <csignal>

namespace nexilis
{

/// The file path we are reading messages from.
static std::string path;

UnixSocketServer::UnixSocketServer(const std::string& socketPath)
{
    path = socketPath;
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
    ssize_t bytesRead = recvfrom(m_serverSocket, m_buffer, m_bufferSize, 0, (struct sockaddr *)&clientAddress, &clientAddressLen);

    if (bytesRead > 0)
    {
        std::cout << "Received message from " << inet_ntoa(clientAddress.sin_addr) << ": " << m_buffer << std::endl;
    }
}

void UnixSocketServer::createSocket()
{
    m_serverSocket = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (m_serverSocket == -1)
    {
        std::cerr << "Error creating socket" << std::endl;
    }
}

void UnixSocketServer::bindSocket()
{
    struct sockaddr_un serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sun_family = AF_UNIX;
    strncpy(serverAddr.sun_path, path.c_str(), sizeof(serverAddr.sun_path) - 1);

    if (bind(m_serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
    {
        std::cerr << "Error binding socket" << std::endl;
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
            std::cout << "File deleted successfully:" << "/tmp/nexilis" << std::endl;
        }
        else
        {
            perror("Error deleting file");
        }
        std::exit(signum);
    }
}

}
