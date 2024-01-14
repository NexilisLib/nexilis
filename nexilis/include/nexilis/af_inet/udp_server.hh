#ifndef NEXILIS_UDP_SERVER_HH
#define NEXILIS_UDP_SERVER_HH

#include <nexilis/af_inet/base_udp_server.hh>
#include <nexilis/message_handler.hh>

namespace nexilis::af_inet
{

class UDPServer : public BaseUDPServer
{
public:
    /// Constructor.
    /// \param port The port we are assigning the udp server.
    /// This has been initialized the value of Port::UDP.
    UDPServer(unsigned port = static_cast<unsigned>(Port::UDP)) : BaseUDPServer(port)
    {
    }

    /// Destructor.
    ~UDPServer()
    {
    }

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
        return Type::UDP;
    }

private:
    MessageHandler m_messageHandler;
};


} // namespace nexilis::af_inet

#endif
