#ifndef NEXILIS_BOOST_UDP_SERVER_HH
#define NEXILIS_BOOST_UDP_SERVER_HH

#include <nexilis/authentication.hh>
#include <nexilis/loggable.hh>
#include <nexilis/protocol.hh>
#include <nexilis/server_protocol.hh>
#include <nexilis/command.hh>

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/udp.hpp>

#include <thread>

namespace nexilis
{

class BoostUDPServer : public Protocol,
                       public ServerProtocol,
                       public Command
{
public:
    /// Constructor.
    BoostUDPServer(const Authentication& authentication, int port);

    /// Destructor.
    ~BoostUDPServer();

    /// Move constructor.
    BoostUDPServer(BoostUDPServer&& other);

    /// Move assignment operator.
    BoostUDPServer& operator=(BoostUDPServer&& other);

    /// Deleted copy constructor.
    BoostUDPServer(const BoostUDPServer&) = delete;

    /// Deleted copy assignment operator.
    BoostUDPServer& operator=(const BoostUDPServer&) = delete;

    /// Protocol::start() implementation.
    void start() override;

    /// Protocol::stop() implementation.
    void stop() override
    {
        Log::error("Not implemented");
    }

    /// Protocol::getType() implementation.
    Type getType() override
    {
        return Type::BOOST_UDP_SERVER;
    }

private:
    void receiveFromClients();

private:
    std::unique_ptr<boost::asio::io_context> m_ioContext;
    std::unique_ptr<std::mutex> m_mutex;
    boost::asio::ip::udp::endpoint m_remoteEndpoint;
    boost::asio::ip::udp::socket m_socket;
    std::vector<uint8_t> m_receiveBuffer;
    std::thread m_ioContextThread;
    std::thread m_receiveThread;
};

} // namespace nexilis

#endif
