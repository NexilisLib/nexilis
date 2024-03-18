#ifndef NEXILIS_AF_INET_UDP_SERVER_HH
#define NEXILIS_AF_INET_UDP_SERVER_HH

#include "nexilis/server_protocol.hh"
#include <nexilis/af_inet/base_udp_server.hh>
#include <nexilis/message_handler.hh>

namespace nexilis::af_inet
{

class UDPServer : public BaseUDPServer, public ServerProtocol
{
public:
    /// Constructor.
    /// \param port The port we are assigning the udp server.
    /// This has been initialized the value of Port::UDP.
    UDPServer(unsigned port = static_cast<unsigned>(Port::UDP));

    /// Destructor.
    ~UDPServer();

    /// Move constructor.
    UDPServer(UDPServer&& other);

    /// Move assignment operator.
    UDPServer& operator=(UDPServer&& other);

    /// Deleted copy constructor.
    UDPServer(const UDPServer& other) = delete;

    /// Deleted copy assignment operator.
    UDPServer& operator=(const UDPServer& other) = delete;

    /// Protocol::start() implementation.
    void start() override;

    /// Protocol::stop() implementation.
    void stop() override
    {
        BaseUDPServer::stop();
    }

    // Get message from server.
    // \return Message from the BaseUdpServer.
    BaseUDPServer::Message getNextMessage()
    {
        BaseUDPServer::Message msg;
        BaseUDPServer::getNextMessage(msg);
        return msg;
    }

    /// Protocol::getType() implementation.
    Type getType() override
    {
        return Type::AF_INET_UDP_SERVER;
    }

    void sendDataToClient(const std::vector<uint8_t>& data, const sockaddr* clientAddr, socklen_t clientAddrLen);

private:
    std::thread m_receiveThread;
};

} // namespace nexilis::af_inet

#endif
