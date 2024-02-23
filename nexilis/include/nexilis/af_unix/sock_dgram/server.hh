#ifndef NEXILIS_UNIX_SOCKET_SERVER_HH
#define NEXILIS_UNIX_SOCKET_SERVER_HH

#include <nexilis/protocol.hh>

namespace nexilis::af_unix::sock_dgram
{

class Server : public Protocol
{
public:
    /// Constructor.
    Server(const std::string& socketPath);

    /// Destructor.
    ~Server();

    void start() override
    {
        while (true)
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
    void createSocket();
    void bindSocket();
    void receiveMessage();
    static void signalHandler(int signum);
private:
    int m_serverSocket;
    int m_bufferSize;
    char* m_buffer;
};

} // namespace nexilis::af_unix::sock_dgram

#endif
