#ifndef NEXILIS_AF_UNIX_SOCK_STREAM_SERVER_HH
#define NEXILIS_AF_UNIX_SOCK_STREAM_SERVER_HH

#include <nexilis/protocol.hh>
#include <nexilis/message_handler.hh>

#include <cstdint>
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

    /// Move constructor.
    Server(Server&& other);

    /// Move assignment operator.
    Server& operator=(Server&& other);

    /// Deleted copy constructor.
    Server(const Server& other) = delete;

    /// Deleted copy assignment.
    Server& operator=(const Server& other) = delete;

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
        return Type::AF_UNIX_SOCK_STREAM_SERVER;
    }

    void sendMessage(int clientSocket, const std::vector<uint8_t>& message);

private:
    void createSocket();
    void bindSocket();

private:
    std::string m_socketPath;
    int m_serverSocket;
    char* m_buffer;

};

}

#endif
