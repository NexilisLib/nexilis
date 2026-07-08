#ifdef __linux__

#ifndef NEXILIS_AF_UNIX_SOCK_STREAM_SERVER_HH
#define NEXILIS_AF_UNIX_SOCK_STREAM_SERVER_HH

#include <nexilis/protocol.hh>
#include <nexilis/server/server_config.hh>
#include <nexilis/server/server_protocol.hh>

#include <thread>

namespace nexilis::server::af_unix
{

class StreamServer : public Protocol,
                     public ServerProtocol
{
public:
    /// Constructor.
    StreamServer(const ServerConfig& settings, const std::string& socketPath);

    /// Destructor.
    ~StreamServer();

    /// Move constructor.
    StreamServer(StreamServer&& other);

    /// Move assignment operator.
    StreamServer& operator=(StreamServer&& other);

    /// Deleted copy constructor.
    StreamServer(const StreamServer& other) = delete;

    /// Deleted copy assignment.
    StreamServer& operator=(const StreamServer& other) = delete;

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
    void sendMessage(int clientSocket, const nx_data& message);

private:
    std::string m_socketPath;
    int m_serverSocket;
    nx_data m_buffer;
    std::thread m_receiveThread;
};

} // namespace nexilis::server::af_unix

#endif // NEXILIS_AF_UNIX_SOCK_STREAM_SERVER_HH

#endif // __linux__
