#ifndef NEXILIS_UNIX_SOCKET_SERVER_HH
#define NEXILIS_UNIX_SOCKET_SERVER_HH

#include <nexilis/message_handler.hh>
#include <nexilis/protocol.hh>
#include <string>

namespace nexilis::af_unix
{

class UnixSocketServer : public Protocol
{
public:
    /// Constructor.
    UnixSocketServer(const std::string& socketPath);

    /// Destructor.
    ~UnixSocketServer();

    // Read messages from the specified path.
    void receiveMessage();

    void start() override
    {
        while(true)
        {
            receiveMessage();
        }
    }

    void stop() override
    {
    }

    Type getType() override
    {
        return Type::AF_UNIX_SOCK_DGRAM_SERVER;
    }

private:
    int m_serverSocket;
    int m_bufferSize;
    char* m_buffer;

    void createSocket();
    void bindSocket();
    static void signalHandler(int signum);
};

} // namespace nexilis::af_unix

#endif
