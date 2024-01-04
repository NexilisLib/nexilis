#ifndef NEXILIS_UDP_CLIENT_RECEIVER_HH
#define NEXILIS_UDP_CLIENT_RECEIVER_HH

#include <nexilis/af_inet/base_udp_server.hh>
#include <nexilis/log.hh>

namespace nexilis
{

class UdpClientReceiver : public BaseUdpServer
{
public:
    UdpClientReceiver(unsigned port) : BaseUdpServer(port)
    {
    }

    void start() override
    {
        auto msg = BaseUdpServer::receiveMessage();
        Log::info("Client received message: ", msg.message);
    }

    /// Protocol::stop() implementation.
    void stop() override
    {
    }

    /// Protocol::getType() implementation.
    Type getType() override
    {
        return Type::UDP;
    }

};

}

#endif
