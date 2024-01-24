#ifndef NEXILIS_AF_INET_TCP_SERVER_HH
#define NEXILIS_AF_INET_TCP_SERVER_HH

#include <nexilis/protocol.hh>
#include <nexilis/message_handler.hh>

#include <netinet/in.h>

namespace nexilis::af_inet
{

class TCPServer : public Protocol
{
public:
    struct Client
    {
        std::string address;
        uint16_t port;
        int socket;
    };

    /// Constructor.
    TCPServer(int port);

    /// Destructor.
    ~TCPServer();

    bool startListening();

    Client acceptClient();

    bool sendToClient(int clientSocket, const char* data, size_t dataSize);

    Type getType() override
    {
        return Type::TCP;
    }

    void start() override;
private:
    MessageHandler m_messageHandler;

    int m_serverSocket;

    sockaddr_in m_serverAddr;
};

}

#endif