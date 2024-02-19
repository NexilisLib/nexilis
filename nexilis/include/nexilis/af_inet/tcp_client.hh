#ifndef NEXILIS_AF_INET_TCP_CLIENT_HH
#define NEXILIS_AF_INET_TCP_CLIENT_HH

#include <nexilis/client_protocol.hh>
#include <nexilis/protocol.hh>
#include <nexilis/client_api.hh>

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

    /// Move constructor.
    TCPClient(TCPClient&& other);

    /// Move assignment operator.
    TCPClient& operator=(TCPClient&& other);

    /// Deleted copy constructor.
    TCPClient(const TCPClient& other) = delete;

    /// Deleted copy assigment operator.
    TCPClient& operator=(const TCPClient& other) = delete;

    /// Protocol::start() implementation.
    void start() override;

    /// Protocol::stop() implementation.
    void stop() override
    {
    }

    /// Protocol::getType() implementation.
    Type getType() override
    {
        return Type::AF_INET_TCP_CLIENT;
    }

    /// ClientProtocol::sendMessage(const std::string&) implementation.
    void sendMessage(const std::string& message);

    /// ClientProtocol::sendMessage(const std::vector<uint8_t>&) implementation.
    void sendMessage(const std::vector<uint8_t>& message);

private:
    bool connectToServer();
    bool send(const char* data, size_t dataSize);
    bool receive(char* buffer, size_t bufferSize);
private:
    int m_clientSocket;
    sockaddr_in m_serverAddr;
};

} // namespace nexilis::af_inet

#endif
