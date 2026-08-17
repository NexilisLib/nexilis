#ifndef NEXILIS_BOOST_UDP_CLIENT_HH
#define NEXILIS_BOOST_UDP_CLIENT_HH

#include <nexilis/client/client_protocol.hh>
#include <nexilis/protocol.hh>

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/udp.hpp>

#include <future>
#include <memory>
#include <mutex>
#include <thread>
#include <unordered_map>

namespace nexilis::client::nxboost
{

class UDPClient : public virtual NxClass,
                  public Protocol,
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

    /// ClientProtocol::sendMessage(const nx_data&, const std::function<void()>&) implementation.
    void sendMessage(const nx_data& message, const std::function<void()>& callback) override;

    /// ClientProtocol::sendMessageAsync(const nx_data&) implementation.
    std::future<void> sendMessageAsync(const nx_data& message) override;

private:
    void receiveLoop();

private:
    std::unique_ptr<std::atomic<bool>> m_stopped;
    std::unique_ptr<boost::asio::io_context> m_ioContext;
    std::thread m_ioContextThread;
    std::thread m_receiveMessageThread;
    std::unique_ptr<std::mutex> m_mutex;
    boost::asio::ip::udp::socket m_socket;
    std::vector<char> m_receiveBuffer;
    boost::asio::ip::udp::endpoint m_remoteEndpoint;
    boost::asio::ip::udp::endpoint m_sendEndpoint;

    std::unordered_map<uint64_t, std::shared_ptr<std::promise<void>>> m_pendingSends;
    std::shared_ptr<std::mutex> m_pendingSendsMutex;
};

} // namespace nexilis::client::nxboost

#endif
