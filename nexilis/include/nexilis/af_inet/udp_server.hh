#ifndef NEXILIS_UDP_SERVER_HH
#define NEXILIS_UDP_SERVER_HH

namespace nexilis
{

class UDPServer
{
public:
    /// Constructor.
    UDPServer();

    /// Destructor.
    ~UDPServer();

    /// Listen to messages.
    void receiveMessage();

private:
    int m_serverSocket;
};

}

#endif
