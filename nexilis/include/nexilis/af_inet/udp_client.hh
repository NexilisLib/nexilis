#ifndef NEXILIS_AF_INET_UDP_CLIENT_HH
#define NEXILIS_AF_INET_UDP_CLIENT_HH

#include <nexilis/protocol.hh>
#include <nexilis/client_api.hh>
#include <nexilis/client_protocol.hh>

#include <sys/un.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <thread>

namespace nexilis::af_inet
{

class UDPClient : public Protocol, public ClientProtocol
{
public:
    UDPClient(ClientAPI& api);

    /// Move constructor.
    UDPClient(UDPClient&& other);

    /// Move assignment operator.
    UDPClient& operator=(UDPClient&& other);

    /// Deleted copy constructor.
    UDPClient(const UDPClient& other) = delete;

    /// Deleted copy assignment operator.
    UDPClient& operator=(const UDPClient& other) = delete;

    /// ClientProtocol::sendMessage(const std::string&) implementation.
    void sendMessage(const std::string& message) override;

    /// ClientProtocol::sendMessage(const std::vector<uint8_t>&) implementation.
    void sendMessage(const std::vector<uint8_t>& message) override;

    // Send data using UDP
    void sendData(const char* data, size_t dataSize);

    std::vector<uint8_t> receiveData(sockaddr* srcAddr, socklen_t* srcAddrLen);

    void start() override;

    /// Protocol::stop() implementation.
    void stop() override;

    /// Protocol::getType() implementation.
    Type getType() override
    {
        return Type::AF_INET_UDP_CLIENT;
    }

private:
    int createSocket();
    void receiveLoop();

private:
    int m_clientSocket;
    sockaddr_in m_serverAddr;

    std::thread m_receiverThread;

    ClientAPI& m_api;
};

} // namespace nexilis::af_inet

#endif
