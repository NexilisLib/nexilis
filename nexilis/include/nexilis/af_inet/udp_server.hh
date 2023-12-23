#ifndef NEXILIS_UDP_SERVER_HH
#define NEXILIS_UDP_SERVER_HH

#include <nexilis/ports.hh>
#include <nexilis/protocol.hh>

namespace nexilis
{

class UDPServer : public Protocol
{
public:
    /// Constructor.
    /// \param port The port we are assigning the udp server.
    /// This has been initialized the value of Port::UDP.
    UDPServer(unsigned port = static_cast<unsigned>(Port::UDP));

    /// Destructor.
    ~UDPServer();

    /// Listen to messages.
    /// TODO change to Protocol::start();
    /// also prolly implement Protocol::stop()
    void receiveMessage();

private:
    int m_serverSocket;
};

} // namespace nexilis

#endif
