#ifndef NEXILIS_UNIX_SOCKET_SENDER_HH
#define NEXILIS_UNIX_SOCKET_SENDER_HH

#include <nexilis/connection.hh>

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
    void sendMessage(const std::string& message);

private:
    int m_socket_fd;

    /// The path where the message is sent.
    std::string m_socketPath;

};

}

#endif
