#ifndef NEXILIS_TCP_CLIENT_HH
#define NEXILIS_TCP_CLIENT_HH

#include <nexilis/client/protocol/nxboost/tcp_client.hh>
#include <nexilis/protocol.hh>

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

    /// Get the Protocol::Type.
    static Protocol::Type getType()
    {
        return Protocol::Type::BOOST_TCP_CLIENT;
    }

    /// Start running nexilis tcp client.
    void start();

    /// Stop running nexilis tcp client.
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
