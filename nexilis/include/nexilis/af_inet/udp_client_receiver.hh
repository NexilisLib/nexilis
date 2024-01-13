#ifndef NEXILIS_UDP_CLIENT_RECEIVER_HH
#define NEXILIS_UDP_CLIENT_RECEIVER_HH

#include <nexilis/af_inet/base_udp_server.hh>
#include <nexilis/log.hh>
#include <nexilis/command.hh>
#include <nexilis/client_protocol.hh>

namespace nexilis
{

class UdpClientReceiver : public BaseUdpServer, public ClientProtocol
{
public:
    UdpClientReceiver(unsigned port) : BaseUdpServer(port)
    {
    }

    void start() override
    {
        BaseUdpServer::start();

        while (true)
        {
            BaseUdpServer::Message msg;

            if (BaseUdpServer::getNextMessage(msg))
            {
                Log::info("Received message: ", msg.message, " from ", msg.address);
            }
        }
    }

    /// Protocol::stop() implementation.
    void stop() override
    {
        BaseUdpServer::stop();
    }

    /// Protocol::getType() implementation.
    Type getType() override
    {
        return Type::UDP;
    }
};

}

#endif
