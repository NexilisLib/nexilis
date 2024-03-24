#ifndef NEXILIS_BOOST_TCP_SERVER_HH
#define NEXILIS_BOOST_TCP_SERVER_HH

#include <nexilis/loggable.hh>
#include <nexilis/message_handler.hh>
#include <nexilis/protocol.hh>
#include <nexilis/server_protocol.hh>

#include <boost/asio.hpp>
#include <boost/json.hpp>

namespace nexilis::boost
{
namespace boost = ::boost;

class TCPServer : public Protocol,
                  public ServerProtocol
{
public:
    /// Constructor.
    TCPServer(int serverPort);

    /// Destructor.
    ~TCPServer();

    /// Move constructor.
    TCPServer(TCPServer&& other);

    /// Move assignment operator.
    TCPServer& operator=(TCPServer&& other);

    /// Deleted move constructor.
    TCPServer(const TCPServer&) = delete;

    /// Deleted move assignment operator.
    TCPServer& operator=(const TCPServer&) = delete;

    /// Protocol::start() implementation.
    void start() override;

    /// Protocol::stop() implementation.
    void stop() override;

    /// Protocol::getType() implementation.
    Type getType() override
    {
        return Type::BOOST_TCP_SERVER;
    }

private:
    bool sendToClient(const std::string& data, boost::asio::ip::tcp::socket& clientSocket);
    bool startListening();
    bool acceptClients();

private:
    std::unique_ptr<std::mutex> m_mutex;
    std::unique_ptr<boost::asio::io_context> m_ioContext;

    boost::asio::ip::tcp::acceptor m_acceptor;
    boost::asio::ip::tcp::socket m_socket;

    std::thread m_listenThread;
    std::thread m_ioContextThread;
};

} // namespace nexilis::boost

#endif
