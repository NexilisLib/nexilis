#ifndef NEXILIS_UDP_SERVER_HH
#define NEXILIS_UDP_SERVER_HH

#include <nexilis/af_inet/base_udp_server.hh>

namespace nexilis
{

class AfInetUdpServer : public BaseUdpServer
{
public:
    /// Constructor.
    /// \param port The port we are assigning the udp server.
    /// This has been initialized the value of Port::UDP.
    AfInetUdpServer(unsigned port = static_cast<unsigned>(Port::UDP));

    /// Destructor.
    ~AfInetUdpServer();

    /// Protocol::start() implementation.
    void start() override;

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

} // namespace nexilis

#endif
