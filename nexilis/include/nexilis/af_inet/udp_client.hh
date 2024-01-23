#ifndef NEXILIS_UDP_CLIENT_HH
#define NEXILIS_UDP_CLIENT_HH

#include <nexilis/client_protocol.hh>
#include <nexilis/client_api/client_api.hh>

namespace nexilis::af_inet
{

class UDPClient : public Protocol, public ClientProtocol
{
public:
    UDPClient(ClientAPI& api);

    void sendMessage(const std::string& message) override;

    // Send data using UDP
    void sendData(const char* data, size_t dataSize);

    void receiveData(char* buffer, size_t bufferSize, struct sockaddr* srcAddr, socklen_t* srcAddrLen);

    void attach();

    void start() override;

    /// Protocol::stop() implementation.
    void stop() override;

    /// Protocol::getType() implementation.
    Type getType() override
    {
        return Type::UDP;
    }

private:
    int createSocket();
    void receiveLoop();

private:
    int m_clientSocket;
    struct sockaddr_in m_serverAddr;

    std::thread m_receiverThread;

    ClientAPI& m_api;
};

} // namespace nexilis::af_inet

#endif
