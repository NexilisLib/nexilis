#ifndef NEXILIS_BOOST_TCP_SERVER_HH
#define NEXILIS_BOOST_TCP_SERVER_HH

#include <nexilis/loggable.hh>
#include <nexilis/message_handler.hh>
#include <nexilis/protocol.hh>
#include <nexilis/server_protocol.hh>

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>

#include <thread>

namespace nexilis
{

class BoostTCPServer : public Protocol,
                       public ServerProtocol,
                       public Loggable
{
public:
    /// Constructor.
    BoostTCPServer(int serverPort);

    /// Destructor.
    ~BoostTCPServer();

    /// Move constructor.
    BoostTCPServer(BoostTCPServer&& other);

    /// Move assignment operator.
    BoostTCPServer& operator=(BoostTCPServer&& other);

    /// Deleted move constructor.
    BoostTCPServer(const BoostTCPServer&) = delete;

    /// Deleted move assignment operator.
    BoostTCPServer& operator=(const BoostTCPServer&) = delete;

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
    bool sendToClient(const std::vector<uint8_t>& data, boost::asio::ip::tcp::socket& clientSocket);
    bool startListening();
    bool acceptClients();

private:
    std::unique_ptr<std::mutex> m_mutex;
    std::unique_ptr<boost::asio::io_context> m_ioContext;

    boost::asio::ip::tcp::acceptor m_acceptor;

    std::thread m_listenThread;
    std::thread m_ioContextThread;
};

} // namespace nexilis

#endif
