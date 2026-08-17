#ifndef NEXILIS_UDP_CLIENT_HH
#define NEXILIS_UDP_CLIENT_HH

#include <nexilis/client/protocol/nxboost/udp_client.hh>

namespace nexilis
{

class UDPClient
{
public:
    /// Constructor.
    explicit UDPClient(client::ClientAPI& api);

    /// Move constructor.
    UDPClient(UDPClient&& other) noexcept;

    /// Move assignment operator.
    UDPClient& operator=(UDPClient&& other) noexcept;

    /// Deleted copy constructor.
    UDPClient(const UDPClient&) = delete;

    /// Deleted copy assignment operator.
    UDPClient& operator=(const UDPClient&) = delete;

    /// Start running the UDP client.
    void start();

    /// Stop running the UDP client.
    void stop();

    /// Send nexilis message.
    void sendMessage(const nx_data& message);

    /// Send nexilis message with a callback.
    void sendMessage(const nx_data& message, const std::function<void()>& callback);

    /// Send async nexilis message.
    std::future<void> sendMessageAsync(const nx_data& message);

private:
    client::nxboost::UDPClient m_udpClient;
};

} // namespace nexilis

#endif
