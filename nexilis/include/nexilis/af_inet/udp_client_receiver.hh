#ifndef NEXILIS_UDP_CLIENT_RECEIVER_HH
#define NEXILIS_UDP_CLIENT_RECEIVER_HH

#include <nexilis/af_inet/base_udp_server.hh>
#include <nexilis/log.hh>
#include <nexilis/command.hh>

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
        BaseUdpServer::start();

        while (true)
        {
            BaseUdpServer::Message msg;

            if (BaseUdpServer::getNextMessage(msg))
            {
                Log::info("Received message: ", msg.message, " from ", msg.m_address);

                Connection connection(msg.m_address);
                if (!Command::read(msg.message.c_str(), msg.message.size(), connection, *this))
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
