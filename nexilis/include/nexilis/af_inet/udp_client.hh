#ifndef NEXILIS_AF_INET_UDP_CLIENT_HH
#define NEXILIS_AF_INET_UDP_CLIENT_HH

#include <nexilis/protocol.hh>
#include <nexilis/client_api/client_api.hh>
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

    void sendMessage(const std::string& message) override;

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
