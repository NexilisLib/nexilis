#ifndef NEXILIS_BOOST_UDP_SERVER_HH
#define NEXILIS_BOOST_UDP_SERVER_HH

#include <nexilis/protocol.hh>

#include <boost/asio.hpp>

namespace nexilis::boost
{

class UDPServer : public Protocol
{
public:
    /// Constructor.
    UDPServer(int port);

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
    }

    /// Protocol::getType() implementation.
    Type getType()
    {
        return Type::BOOST_UDP_SERVER;
    }

private:
    void receiveFromClients();

private:
    std::unique_ptr<::boost::asio::io_context> m_ioContext;
    std::unique_ptr<std::mutex> m_mutex;
    ::boost::asio::ip::udp::socket m_socket;
    ::boost::asio::ip::udp::endpoint m_remoteEndpoint;
    std::vector<char> m_receiveBuffer;
    std::thread m_ioContextThread;
};

}

#endif
