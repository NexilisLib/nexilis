#ifndef NEXILIS_UNIX_SOCKET_SENDER_HH
#define NEXILIS_UNIX_SOCKET_SENDER_HH

#include <string>

namespace nexilis
{

class UnixSocketSender
{
public:
    /// Constructor.
    UnixSocketSender(const std::string& socketPath);

    /// Destructor.
    ~UnixSocketSender();

    /// Send message to path given in constructor.
    void sendMessageToServer(const std::string& message);
    void sendMessageToClient(const std::string& message, const std::string& clientPath);

private:
    /// Send message to path given in constructor.
    void sendMessage(const std::string& message, const std::string& path);

    int m_socket_fd;

    /// The path where the message is sent.
    std::string m_socketPath;
};

} // namespace nexilis

#endif
