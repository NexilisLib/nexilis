#ifndef NEXILIS_BOOST_UDP_CLIENT_HH
#define NEXILIS_BOOST_UDP_CLIENT_HH

#include <nexilis/client_protocol.hh>
#include <nexilis/loggable.hh>
#include <nexilis/protocol.hh>

#include <boost/asio.hpp>

namespace nexilis::boost
{
namespace boost = ::boost;

class UDPClient : public Protocol,
                  public ClientProtocol,
                  public Loggable
{
public:
    /// Constructor.
    UDPClient(ClientAPI& api);

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
    void stop() override
    {
        Log::error("Not implemented");
    }

    /// Protocol::getType() implementation.
    Type getType() override
    {
        return Type::BOOST_UDP_CLIENT;
    }

    /// ClientProtocol::sendMessage(const std::string& message) implementation.
    void sendMessage(const std::string& message) override;

    /// ClientProtocol::sendMessage(const std::vector<uint8_t>& message) implementation.
    void sendMessage(const std::vector<uint8_t>& message) override;

private:
    /// Internal sendMessage function.
    void send(const std::string& message);

private:
    std::thread m_ioContextThread;

private:
    std::unique_ptr<::boost::asio::io_context> m_ioContext;
    std::unique_ptr<std::mutex> m_mutex;
    boost::asio::ip::udp::endpoint m_endpoint;
    boost::asio::ip::udp::endpoint m_remoteEndpoint;
    boost::asio::ip::udp::socket m_socket;
    std::vector<char> m_receiveBuffer;
};

} // namespace nexilis::boost

#endif
