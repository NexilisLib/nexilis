#ifndef NEXILIS_TCP_CLIENT_HH
#define NEXILIS_TCP_CLIENT_HH

#include <nexilis/client/protocol/nxboost/tcp_client.hh>

namespace nexilis
{

class TCPClient
{
public:
    /// Constructor.
    explicit TCPClient(client::ClientAPI& client_api);

    /// Move constructor.
    TCPClient(TCPClient&& other) noexcept;

    /// Move assignment operator.
    TCPClient& operator=(TCPClient&& other) noexcept;

    /// Start running nexilis client.
    void start();

    /// Stop running nexilis client.
    void stop();

    /// Send nexilis message
    void sendMessage(const nx_data& message);

    /// Send nexilis message with custom callback.
    void sendMessage(const nx_data& message,
                     const std::function<void()>& callback);

    std::future<void> sendMessageAsync(const nexilis::nx_data& message);

    nexilis::client::ProtocolStatus getProtocolStatus();

private:
    nexilis::client::nxboost::TCPClient m_tcpClient;
};

} // namespace nexilis

#endif
