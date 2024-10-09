#ifndef NEXILIS_UNIX_SOCKET_CLIENT_HH
#define NEXILIS_UNIX_SOCKET_CLIENT_HH

#include <nexilis/client/client_api.hh>
#include <nexilis/client/client_protocol.hh>
#include <nexilis/protocol.hh>

#include <sys/un.h>

namespace nexilis::af_unix::sock_dgram
{

class Client : public Protocol,
               public ClientProtocol
{
public:
    /// Constructor.
    Client(ClientAPI& clientApi);

    /// Destructor.
    ~Client();

    /// Send message to the server.
    void sendMessage(const std::vector<uint8_t>& message) override;

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

} // namespace nexilis::af_unix::sock_dgram

#endif
