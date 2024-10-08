#ifndef NEXILIS_AF_UNIX_SOCK_STREAM_SERVER_HH
#define NEXILIS_AF_UNIX_SOCK_STREAM_SERVER_HH

#include <nexilis/authentication.hh>
#include <nexilis/loggable.hh>
#include <nexilis/protocol.hh>
#include <nexilis/server_protocol.hh>
#include <nexilis/command.hh>

#include <thread>

namespace nexilis::af_unix::sock_stream
{

class Server : public Protocol,
               public ServerProtocol,
               public Command
{
public:
    /// Constructor.
    Server(const Authentication& authentication, const std::string& socketPath);

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

    /// Protocol start() implementation.
    void start() override;

    void stop() override
    {
    }

    Type getType() override
    {
        return Type::AF_UNIX_SOCK_STREAM_SERVER;
    }

private:
    void createSocket();
    void bindSocket();
    void handleMessages();
    std::string receiveMessage(int socket);
    void sendMessage(int clientSocket, const std::vector<uint8_t>& message);

private:
    std::string m_socketPath;
    int m_serverSocket;
    std::vector<uint8_t> m_buffer;
    std::thread m_receiveThread;
};

} // namespace nexilis::af_unix::sock_stream

#endif
