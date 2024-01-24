#ifndef NEXILIS_UNIX_SOCKET_CLIENT_HH
#define NEXILIS_UNIX_SOCKET_CLIENT_HH

#include <nexilis/client_protocol.hh>
#include <nexilis/client_api/client_api.hh>
#include <nexilis/protocol.hh>

#include <sys/un.h>

namespace nexilis::af_unix
{

class UnixSocketClient : public Protocol, public ClientProtocol
{
public:
    /// Constructor.
    UnixSocketClient(ClientAPI& api);

    /// Destructor.
    ~UnixSocketClient();

    /// Send message to the server.
    void sendMessage(const std::string& message) override;

    std::string receiveMessage();

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

private:
    ClientAPI& m_api;

private:
    // TODO create unixSocket.hh
    std::string m_serverSocketPath;
    int m_clientSocket;
    struct sockaddr_un m_serverAddr;
};

}

#endif
