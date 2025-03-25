#ifndef NEXILIS_BOOST_UDP_CLIENT_HH
#define NEXILIS_BOOST_UDP_CLIENT_HH

#include <nexilis/client/client_protocol.hh>
#include <nexilis/protocol.hh>

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/udp.hpp>

#include <thread>

namespace nexilis::client::nxboost
{

class UDPClient : public Protocol,
                  public ClientProtocol
{
public:
    /// Constructor.
    explicit UDPClient(ClientAPI& api);

    /// Destructor.
    ~UDPClient();

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
    void stop() override;

    /// Protocol::getType() implementation.
    Type getType() override
    {
        return Type::BOOST_UDP_CLIENT;
    }

    /// ClientProtocol::sendMessage(const nx_data& message) implementation.
    void sendMessage(const nx_data& message) override;

private:
    void receiveLoop();

private:
    std::thread m_ioContextThread;
    std::thread m_receiveMessageThread;

private:
    std::unique_ptr<boost::asio::io_context> m_ioContext;
    std::unique_ptr<std::mutex> m_mutex;
    boost::asio::ip::udp::socket m_socket;
    std::vector<char> m_receiveBuffer;
    boost::asio::ip::udp::endpoint m_remoteEndpoint;
};

} // namespace nexilis::client::nxboost

#endif
