#ifndef NEXILIS_AF_INET_TCP_CLIENT_HH
#define NEXILIS_AF_INET_TCP_CLIENT_HH

#include <nexilis/client_protocol.hh>
#include <nexilis/protocol.hh>
#include <nexilis/client_api/client_api.hh>

#include <netinet/in.h>

namespace nexilis::af_inet
{

class TCPClient : public Protocol, public ClientProtocol
{
public:
    /// Constructor.
    TCPClient(ClientAPI& api);

    /// Destructor.
    ~TCPClient();

    bool connectToServer();

    bool send(const char* data, size_t dataSize);

    bool receive(char* buffer, size_t bufferSize);

    void start() override
    {
    }

    /// Protocol::stop() implementation.
    void stop() override
    {
    }

    /// Protocol::getType() implementation.
    Type getType() override
    {
        return Type::TCP;
    }

    void sendMessage(const std::string& message) override
    {
    }

private:
    ClientAPI& m_api;

    int m_clientSocket;
    sockaddr_in m_serverAddr;
};

} // namespace nexilis::af_inet

#endif