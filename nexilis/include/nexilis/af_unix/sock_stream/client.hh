#ifndef NEXILIS_AF_UNIX_SOCK_STREAM_CLIENT_HH
#define NEXILIS_AF_UNIX_SOCK_STREAM_CLIENT_HH

#include <cstdint>
#include <nexilis/protocol.hh>
#include <nexilis/client_protocol.hh>
#include <nexilis/client_api/client_api.hh>

#include <sys/un.h>

namespace nexilis::af_unix::sock_stream
{

class Client : public Protocol, public ClientProtocol
{
public:
    /// Constructor.
    Client(ClientAPI& clientApi);

    /// Destructor.
    ~Client();

    /// Send message to the server.
    /// ClientProtocol::sendMessage implementation.
    void sendMessage(const std::string& message) override;

    /// Receive messages from the server.
    std::vector<uint8_t> receiveMessage();

    /// Protocol::start() implementation.
    void start() override;

    /// Protocol::stop() implementation.
    void stop() override;

    /// Protocol::getType() implementation.
    Protocol::Type getType() override
    {
        return Protocol::Type::UnixSocket;
    }

private:
    // Initialize sockets and stuff, TODO rename
    void createSocket();
    void connectToServer();

private:
    ClientAPI& m_api;

private:
    // TODO create unixSocket.hh
    std::string m_serverSocketPath;
    int m_clientSocket;
    sockaddr_un m_serverAddr;
};

}

#endif
