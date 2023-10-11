#ifndef NEXILIS_UNIX_UDP_SERVER_HH
#define NEXILIS_UNIX_UDP_SERVER_HH

#include "../ports.hh"

#include <arpa/inet.h>
#include <sys/un.h>
#include <unistd.h>

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <csignal>


namespace nexilis
{

// This needs a place somewhere.
const char* unix_socket_path = "/tmp/nexilis";

class UnixUDPServer
{
public:
    /// Constructor.
    UnixUDPServer()
    {
        m_bufferSize = 1024;
        m_buffer = new char[m_bufferSize];
        createSocket();
        bindSocket();

        std::signal(SIGINT, signalHandler);
    }

    /// Destructor.
    ~UnixUDPServer()
    {
        close(m_serverSocket);
        delete[] m_buffer;
    }

    // Receive messages from /tmp/nexilis.
    void receiveMessage()
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

private:
    int m_serverSocket;
    int m_bufferSize;
    char* m_buffer;

    void createSocket()
    {
        m_serverSocket = socket(AF_UNIX, SOCK_DGRAM, 0);
        if (m_serverSocket == -1)
        {
            std::cerr << "Error creating socket" << std::endl;
        }
    }

    void bindSocket()
    {
        struct sockaddr_un serverAddr;
        memset(&serverAddr, 0, sizeof(serverAddr));
        serverAddr.sun_family = AF_UNIX;
        strncpy(serverAddr.sun_path, unix_socket_path, sizeof(serverAddr.sun_path) - 1);

        if (bind(m_serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
        {
            std::cerr << "Error binding socket" << std::endl;
            close(m_serverSocket);
            exit(1);
        }
    }


    static void signalHandler(int signum)
    {
        if (signum == SIGINT)
        {
            // Delete the file before exiting
            if (std::remove(unix_socket_path) == 0)
            {
                std::cout << "File deleted successfully:" << unix_socket_path << std::endl;
            }
            else
            {
                perror("Error deleting file");
            }
            std::exit(signum);
        }
    }

};

}

#endif
