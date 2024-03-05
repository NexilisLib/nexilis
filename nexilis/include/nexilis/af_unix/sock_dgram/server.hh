#ifndef NEXILIS_UNIX_SOCKET_SERVER_HH
#define NEXILIS_UNIX_SOCKET_SERVER_HH

#include <nexilis/protocol.hh>
#include <nexilis/server_protocol.hh>
#include <nexilis/loggable.hh>

namespace nexilis::af_unix::sock_dgram
{

class Server :
            public Protocol,
            public ServerProtocol,
            public Loggable
{
public:
    /// Constructor.
    Server(const std::string& socketPath);

    /// Destructor.
    ~Server();

    /// Move constructor.
    Server(Server&& other);

    /// Move assignment operator.
    Server& operator=(Server&& other);

    /// Deleted copy constructor.
    Server(const Server& other) = delete;

    /// Deleted copy assignment operator.
    Server& operator=(const Server& other) = delete;

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
    std::vector<char> m_buffer;
};

} // namespace nexilis::af_unix::sock_dgram

#endif
