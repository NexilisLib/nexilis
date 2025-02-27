#ifndef NEXILIS_BOOST_TCP_SERVER_HH
#define NEXILIS_BOOST_TCP_SERVER_HH

#include <nexilis/protocol.hh>
#include <nexilis/server/message_handler.hh>
#include <nexilis/server/server_protocol.hh>
#include <nexilis/server/settings.hh>

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>

#include <thread>

namespace nexilis::server::nxboost
{

class TCPServer : public Protocol,
                  public ServerProtocol
{
public:
    /// Constructor.
    TCPServer(const Settings& settings, int serverPort);

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
    bool sendToClient(const nx_data& data, boost::asio::ip::tcp::socket& clientSocket);
    bool startListening();
    bool acceptClients();

private:
    std::unique_ptr<std::mutex> m_mutex;
    std::unique_ptr<boost::asio::io_context> m_ioContext;

    boost::asio::ip::tcp::acceptor m_acceptor;

    std::thread m_listenThread;
    std::thread m_ioContextThread;
};

} // namespace nexilis::server::nxboost

#endif
