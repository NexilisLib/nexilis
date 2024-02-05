#ifndef NEXILIS_BOOST_TCP_SERVER_HH
#define NEXILIS_BOOST_TCP_SERVER_HH

#include <nexilis/protocol.hh>
#include <nexilis/message_handler.hh>

#include <boost/asio.hpp>

namespace nexilis::boost
{

class TCPServer : public Protocol
{
public:
    /// Constructor.
    TCPServer(const std::string& serverPort);

    /// Destructor.
    ~TCPServer();

    /// Move constructor.
    TCPServer(TCPServer&& other);

    /// Move assignment operator.
    TCPServer& operator=(TCPServer&& other);

    TCPServer(const TCPServer&) = delete;
    TCPServer& operator=(const TCPServer&) = delete;

    bool startListening();
    bool acceptClients();
    bool sendToClient(const std::string& data);
    bool receiveFromClient(std::string& buffer);

    Type getType() override
    {
        return Type::BOOST_TCP_SERVER;
    }

    void start() override;

private:
    std::unique_ptr<std::mutex> m_mutex;
    std::unique_ptr<::boost::asio::io_context> m_ioContext;

    ::boost::asio::ip::tcp::acceptor m_acceptor;
    ::boost::asio::ip::tcp::socket m_socket;
};

} // namespace nexilis::boost

#endif
