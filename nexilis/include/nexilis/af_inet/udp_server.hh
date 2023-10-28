#ifndef NEXILIS_UDP_SERVER_HH
#define NEXILIS_UDP_SERVER_HH

#include <nexilis/ports.hh>

namespace nexilis
{

class UDPServer
{
public:
    /// Constructor.
	/// \param port The port we are assigning the udp server.
	/// This has been initialized the value of Port::UDP.
    UDPServer(unsigned port = static_cast<unsigned>(Port::UDP));

    /// Destructor.
    ~UDPServer();

    /// Listen to messages.
    void receiveMessage();

private:
    int m_serverSocket;
};

}

#endif
