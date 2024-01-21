#ifndef NEXILIS_AF_UNIX_SOCK_STREAM_SERVER_HH
#define NEXILIS_AF_UNIX_SOCK_STREAM_SERVER_HH

#include <nexilis/protocol.hh>
#include <nexilis/message_handler.hh>

#include <string>

namespace nexilis::af_unix::sock_stream
{

class Server : public Protocol
{
public:
    /// Constructor.
    Server(const std::string& socketPath);

    /// Destructor.
    ~Server();

    /// Read messages from a specified path.
    void receiveMessage();

    /// Protocol start() implementation.
    /// \note blocking
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
        return Type::UnixSocket;
    }

private:
    std::string m_socketPath;
    int m_serverSocket;
    char* m_buffer;

    void createSocket();
    void bindSocket();

    MessageHandler m_messageHandler;
};

}

#endif
