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
    class Client
    {
    public:
        /// Constructor.
        Client(std::string address, uint16_t port, int socket);

        /// Default constructor.
        Client() = default;

        std::string getAddress()
        {
            return m_address;
        }

        uint16_t getPort()
        {
            return m_port;
        }

        int getSocket()
        {
            return m_socket;
        }

    private:
        std::string m_address;
        uint16_t m_port;
        int m_socket;
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
        return Type::AF_INET_TCP_SERVER;
    }

    void start() override;
private:
    int m_serverSocket;

    sockaddr_in m_serverAddr;
};

}

#endif