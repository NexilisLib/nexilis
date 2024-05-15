#ifndef NEXILIS_BOOST_UDP_CLIENT_HH
#define NEXILIS_BOOST_UDP_CLIENT_HH

#include <nexilis/client_protocol.hh>
#include <nexilis/loggable.hh>
#include <nexilis/protocol.hh>

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/udp.hpp>

#include <thread>

namespace nexilis
{

class BoostUDPClient : public Protocol,
                  public ClientProtocol,
                  public Loggable
{
public:
    /// Constructor.
    BoostUDPClient(ClientAPI& api);

    /// Destructor.
    ~BoostUDPClient();

    /// Move constructor.
    BoostUDPClient(BoostUDPClient&& other);

    /// Move assignment operator.
    BoostUDPClient& operator=(BoostUDPClient&& other);

    /// Deleted copy constructor.
    BoostUDPClient(const BoostUDPClient&) = delete;

    /// Deleted copy assignment operator.
    BoostUDPClient& operator=(const BoostUDPClient&) = delete;

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

    /// ClientProtocol::sendMessage(const std::vector<uint8_t>& message) implementation.
    void sendMessage(const std::vector<uint8_t>& message) override;

private:
    void receiveLoop();

private:
    std::thread m_ioContextThread;
    std::thread m_receiveMessageThread;

private:
    std::unique_ptr<boost::asio::io_context> m_ioContext;
    std::unique_ptr<std::mutex> m_mutex;
    boost::asio::ip::udp::endpoint m_remoteEndpoint;
    boost::asio::ip::udp::socket m_socket;
    std::vector<char> m_receiveBuffer;
};

} // namespace nexilis

#endif
