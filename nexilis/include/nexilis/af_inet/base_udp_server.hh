#ifndef NEXILIS_BASE_UDP_SERVER_HH
#define NEXILIS_BASE_UDP_SERVER_HH

#include <nexilis/protocol.hh>
#include <nexilis/ports.hh>

#include <string>

namespace nexilis
{

class BaseUdpServer : public Protocol
{
public:
    struct Message
    {
        const char* m_address;
        std::string message;
    };

    BaseUdpServer(unsigned port = static_cast<unsigned>(Port::UDP));

    virtual ~BaseUdpServer();

    Message receiveMessage();

private:
    int m_serverSocket;
};

}

#endif
