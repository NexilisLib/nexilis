#ifndef NEXILIS_BOOST_UDP_CLIENT_HH
#define NEXILIS_BOOST_UDP_CLIENT_HH

#include <nexilis/protocol.hh>
#include <nexilis/client_protocol.hh>

#include <boost/asio.hpp>

namespace nexilis::boost
{

class UDPClient : public Protocol, public ClientProtocol
{
public:
    /// Constructor.
    UDPClient(ClientAPI& api);

    /// Move constructor.
    UDPClient(UDPClient&& other);

    /// Move assignment operator.
    UDPClient& operator=(UDPClient&& other);

    /// Deleted copy constructor.
    UDPClient(const UDPClient&) = delete;

    /// Deleted copy assignment operator.
    UDPClient& operator=(const UDPClient&) = delete;

    /// Protocol::start() implementation.
    void start() override;

    /// Protocol::stop() implementation.
    void stop() override
    {
    }

    /// Protocol::getType() implementation.
    Type getType()
    {
        return Type::BOOST_UDP_CLIENT;
    }

    /// ClientProtocol::sendMessage(const std::string& message) implementation.
    void sendMessage(const std::string& message);

    /// ClientProtocol::sendMessage(const std::vector<uint8_t>& message) implementation.
    void sendMessage(const std::vector<uint8_t>& message);

private:
    std::unique_ptr<::boost::asio::io_context> m_ioContext;
    ::boost::asio::ip::udp::endpoint m_endPoint;
    ::boost::asio::ip::udp::endpoint m_remoteEndpoint;
    ::boost::asio::ip::udp::socket m_socket;
    std::vector<char> m_receiveBuffer;
};

}

#endif