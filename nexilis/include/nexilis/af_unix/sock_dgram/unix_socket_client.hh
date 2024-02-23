#ifndef NEXILIS_UNIX_SOCKET_CLIENT_HH
#define NEXILIS_UNIX_SOCKET_CLIENT_HH

#include <nexilis/client_protocol.hh>
#include <nexilis/client_api.hh>
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

    /// Protocol::start() implementation.
    void start() override;

    /// Protocol::stop() implementation.
    void stop() override;

    /// Protocol::getType() implementation.
    Protocol::Type getType() override
    {
        return Protocol::Type::AF_UNIX_SOCK_DGRAM_CLIENT;
    }

private:
    void createSocket();
    std::string receiveMessage();
private:
    std::string m_serverSocketPath;
    int m_clientSocket;
    struct sockaddr_un m_serverAddr;
};

}

#endif
