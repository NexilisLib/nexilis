#ifndef NEXILIS_BOOST_TCP_SERVER_HH
#define NEXILIS_BOOST_TCP_SERVER_HH

#include <nexilis/protocol.hh>

#include <boost/asio.hpp>

namespace nexilis::boost
{

class TCPServer : public Protocol
{
public:
    TCPServer(const std::string& serverPort);
    ~TCPServer();

    bool startListening();
    bool acceptClient();
    bool sendToClient(const std::string& data);
    bool receiveFromClient(std::string& buffer);

    Type getType() override
    {
        return Type::TCP;
    }

private:
    ::boost::asio::io_service m_ioService;
    ::boost::asio::ip::tcp::acceptor m_acceptor;
    ::boost::asio::ip::tcp::socket m_socket;
};

} // namespace nexilis::boost

#endif