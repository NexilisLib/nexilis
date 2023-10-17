#ifndef NEXILIS_UNIX_SOCKET_SENDER_HH
#define NEXILIS_UNIX_SOCKET_SENDER_HH

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include <cstring>
#include <iostream>

namespace nexilis
{

class UnixSocketSender
{
public:
    UnixSocketSender(const std::string& socketPath) :
        m_socketPath(socketPath)
    {
        m_socket_fd = socket(AF_UNIX, SOCK_DGRAM, 0);
        if (m_socket_fd == -1)
        {
            std::cerr<< "Error creating Unix domain socket" << std::endl;
        }
    }

    ~UnixSocketSender()
    {
        close(m_socket_fd);
    }

    void sendMessage(const std::string& message)
    {
        if (m_socket_fd == -1)
        {
            std::cerr << "Socket not initialized properly" << std::endl;
            return;
        }

        struct sockaddr_un server_address;
        std::memset(&server_address, 0, sizeof(struct sockaddr_un));
        server_address.sun_family = AF_UNIX;
        std::strncpy(server_address.sun_path, m_socketPath.c_str(), sizeof(server_address.sun_path) - 1);

        ssize_t bytes_sent = sendto(m_socket_fd, message.c_str(), message.size(), 0,
                (struct sockaddr*)&server_address, sizeof(struct sockaddr_un));

        if (bytes_sent == -1)
        {
            std::cerr << "Error sending message" << std::endl;
        }
    }

private:
    int m_socket_fd;

    /// The path where the message is sent.
    std::string m_socketPath;

};

}

#endif
