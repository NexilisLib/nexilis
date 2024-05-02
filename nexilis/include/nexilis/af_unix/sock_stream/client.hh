#ifndef NEXILIS_AF_UNIX_SOCK_STREAM_CLIENT_HH
#define NEXILIS_AF_UNIX_SOCK_STREAM_CLIENT_HH

#include <nexilis/client_api.hh>
#include <nexilis/client_protocol.hh>
#include <nexilis/protocol.hh>

#include <sys/un.h>

#include <mutex>
#include <thread>

namespace nexilis::af_unix::sock_stream
{

class Client : public Protocol, public ClientProtocol
{
public:
    /// Constructor.
    Client(ClientAPI& clientApi);

    /// Destructor.
    ~Client();

    /// Move constructor.
    Client(Client&& other);

    /// Move assignment operator.
    Client& operator=(Client&& other);

    /// Deleted copy constructor.
    Client(const Client& other) = delete;

    /// Deleted copy assignment operator.
    Client& operator=(const Client& other) = delete;

    /// Protocol::start() implementation.
    void start() override;

    /// Protocol::stop() implementation.
    void stop() override;

    /// Protocol::getType() implementation.
    Protocol::Type getType() override
    {
        return Protocol::Type::AF_UNIX_SOCK_STREAM_CLIENT;
    }

    /// Send message to the server.
    /// ClientProtocol::sendMessage(const std::vector<uint8_t>&) implementation.
    void sendMessage(const std::vector<uint8_t>& message) override;

private:
    // Initialize sockets and stuff.
    void createSocket();
    void connectToServer();

    /// Internal function for sending messages to the server.
    void sendMsg(const std::string& message);

    /// Receive messages from the server.
    std::vector<uint8_t> receiveMessage();

private:
    std::string m_serverSocketPath;
    int m_clientSocket;
    sockaddr_un m_serverAddr;
    std::thread m_receiveThread;
    std::unique_ptr<std::mutex> m_mutex;
};

} // namespace nexilis::af_unix::sock_stream

#endif
