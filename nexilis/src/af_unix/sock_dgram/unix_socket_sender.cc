#include <nexilis/af_unix/sock_dgram/unix_socket_sender.hh>
#include <nexilis/log.hh>

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include <cstring>

namespace nexilis
{

UnixSocketSender::UnixSocketSender(const std::string& socketPath)
    : m_socketPath(socketPath)
{
    m_socket_fd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (m_socket_fd == -1)
    {
        Log::critical("Error creating Unix domain socket");
    }
}

UnixSocketSender::~UnixSocketSender()
{
    close(m_socket_fd);
}

void UnixSocketSender::sendMessageToServer(const std::string& message)
{
    sendMessage(message, m_socketPath);
}

void UnixSocketSender::sendMessageToClient(const std::string& message, const std::string& clientPath)
{
    sendMessage(message, clientPath);
}

void UnixSocketSender::sendMessage(const std::string& message, const std::string& destinationPath)
{
    if (m_socket_fd == -1)
    {
        Log::critical("Unix socket not initialized properly");
        return;
    }

    struct sockaddr_un server_address;
    std::memset(&server_address, 0, sizeof(struct sockaddr_un));
    server_address.sun_family = AF_UNIX;
    std::strncpy(server_address.sun_path, destinationPath.c_str(), sizeof(server_address.sun_path) - 1);

    ssize_t bytes_sent = sendto(m_socket_fd, message.c_str(), message.size(), 0,
                                (struct sockaddr*)&server_address, sizeof(struct sockaddr_un));

    if (bytes_sent == -1)
    {
        Log::error("Error sending message");
    }
}

} // namespace nexilis
