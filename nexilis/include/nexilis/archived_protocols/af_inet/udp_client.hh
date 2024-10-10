#ifndef NEXILIS_AF_INET_UDP_CLIENT_HH
#define NEXILIS_AF_INET_UDP_CLIENT_HH

#include <nexilis/client/client_api.hh>
#include <nexilis/client_protocol.hh>
#include <nexilis/protocol.hh>

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/un.h>

#include <thread>

namespace nexilis::af_inet
{

class UDPClient : public Protocol, public ClientProtocol
{
public:
    /// Constructor.
    UDPClient(ClientAPI& api);

    /// Move constructor.
    UDPClient(UDPClient&& other);

    /// Move assignment operator.
    UDPClient& operator=(UDPClient&& other);

    /// Deleted copy constructor.
    UDPClient(const UDPClient& other) = delete;

    /// Deleted copy assignment operator.
    UDPClient& operator=(const UDPClient& other) = delete;

    /// Protocol::start() implementation.
    void start() override;

    /// Protocol::stop() implementation.
    void stop() override;

    /// Protocol::getType() implementation.
    Type getType() override
    {
        return Type::AF_INET_UDP_CLIENT;
    }

    /// ClientProtocol::sendMessage(const nx_data&) implementation.
    void sendMessage(const nx_data& message) override;

private:
    int createSocket();
    void receiveLoop();

    /// Internal function for sending data.
    void sendData(const char* data, size_t dataSize);

    /// Internal function for receiving data (recvfrom).
    nx_data receiveData(sockaddr* srcAddr, socklen_t* srcAddrLen);

private:
    int m_clientSocket;
    sockaddr_in m_serverAddr;
    std::thread m_receiverThread;
};

} // namespace nexilis::af_inet

#endif
