#ifndef NEXILIS_BOOST_UDP_SERVER_HH
#define NEXILIS_BOOST_UDP_SERVER_HH

#include <nexilis/server/server_protocol.hh>
#include <nexilis/server/settings.hh>
#include <nexilis/logger/loggable.hh>
#include <nexilis/protocol.hh>

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/udp.hpp>

#include <thread>

namespace nexilis::server::nxboost
{

class UDPServer : public Protocol,
                  public ServerProtocol
{
public:
    /// Constructor.
    UDPServer(const Settings& settings, int port);

    /// Destructor.
    ~UDPServer();

    /// Move constructor.
    UDPServer(UDPServer&& other);

    /// Move assignment operator.
    UDPServer& operator=(UDPServer&& other);

    /// Deleted copy constructor.
    UDPServer(const UDPServer&) = delete;

    /// Deleted copy assignment operator.
    UDPServer& operator=(const UDPServer&) = delete;

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

} // namespace nexilis::server::nxboost

#endif
