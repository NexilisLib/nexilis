#ifndef NEXILIS_AF_UNIX_SOCK_STREAM_CLIENT_HH
#define NEXILIS_AF_UNIX_SOCK_STREAM_CLIENT_HH

#include <nexilis/client/client_api.hh>
#include <nexilis/client/client_protocol.hh>
#include <nexilis/protocol.hh>
#include <nexilis/nexilis_constants.hh>

#include <sys/un.h>

#include <mutex>
#include <thread>

namespace nexilis::client::af_unix
{

class StreamClient : public Protocol, public ClientProtocol
{
public:
    /// Constructor.
    StreamClient(ClientAPI& clientApi);

    /// Destructor.
    ~StreamClient();

    /// Move constructor.
    StreamClient(StreamClient&& other);

    /// Move assignment operator.
    StreamClient& operator=(StreamClient&& other);

    /// Deleted copy constructor.
    StreamClient(const StreamClient& other) = delete;

    /// Deleted copy assignment operator.
    StreamClient& operator=(const StreamClient& other) = delete;

    /// Protocol::start() implementation.
    void start() override;

    /// Protocol::stop() implementation.
    void stop() override;

    /// Protocol::getType() implementation.
    Protocol::Type getType() override
    {
        return Protocol::Type::AF_UNIX_SOCK_STREAM_CLIENT;
    }

    /// ClientProtocol::sendMessage(const nx_data&) implementation.
    void sendMessage(const nx_data& message) override;

    /// ClientProtocol::sendMessage(const nx_data&, const std::function<void()>&) implementation.
    void sendMessage(const nx_data& message, const std::function<void()>& callback) override;

private:
    // Initialize sockets and stuff.
    void createSocket();
    void connectToServer();

    /// Internal function for sending messages to the server.
    void sendMsg(const std::string& message);

    /// Receive messages from the server.
    nx_data receiveMessage();

private:
    std::string m_serverSocketPath;
    int m_clientSocket;
    sockaddr_un m_serverAddr;
    std::thread m_receiveThread;
    std::unique_ptr<std::mutex> m_mutex;
};

} // namespace nexilis::client::af_unix

#endif
