#ifndef NEXILIS_UNIX_SOCKET_CLIENT_HH
#define NEXILIS_UNIX_SOCKET_CLIENT_HH

#include <nexilis/client_protocol.hh>
#include <nexilis/client_api/client_api.hh>

#include <sys/un.h>

namespace nexilis::af_unix
{

class UnixSocketClient : public Protocol, public ClientProtocol
{
public:
    /// Constructor.
    UnixSocketClient(ClientAPI& api);

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
    void createSocket();
    void connectToServer();

private:
    ClientAPI& m_api;

private:
    std::string m_serverSocketPath;
    std::string m_clientSocketPath;
    int m_clientSocket;
    struct sockaddr_un m_serverAddr;

};

}

#endif
