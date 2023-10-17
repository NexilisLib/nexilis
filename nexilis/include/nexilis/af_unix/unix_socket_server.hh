#ifndef NEXILIS_UNIX_SOCKET_SERVER_HH
#define NEXILIS_UNIX_SOCKET_SERVER_HH

#include <string>

namespace nexilis
{

class UnixSocketServer
{
public:
    /// Constructor.
    UnixSocketServer(const std::string& socketPath);

    /// Destructor.
    ~UnixSocketServer();

    // Read messages from the specified path.
    void receiveMessage();

private:
    int m_serverSocket;
    int m_bufferSize;
    char* m_buffer;

    void createSocket();

    void bindSocket();

    static void signalHandler(int signum);
};

}

#endif
