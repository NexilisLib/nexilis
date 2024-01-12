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
                Log::info("Received message: ", msg.message, " from ", msg.m_address);

                // This is totally unnecessary abstraction.
                // Command should be refactored to take in an ip address instead.
                // However this client is not at least stored anywhere so we should be fine.
                Client client(msg.m_address);
                if (!Command::read(msg.message.c_str(), msg.message.size(), client, *this, false))
                {
                    Log::error("Received unvalid nexilis command: ", msg.message);
                }
            }
        }
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
