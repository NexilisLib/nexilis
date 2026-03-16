#ifndef NEXILIS_TCP_CLIENT_HH
#define NEXILIS_TCP_CLIENT_HH

#include <nexilis/client/client_api.hh>
#include <nexilis/client/protocol/nxboost/tcp_client.hh>
#include <nexilis/protocol_manager.hh>

namespace nexilis
{

class TCPClient
{
public:
    /// Constructor.
    TCPClient(ProtocolManager* protocolManager, const std::string& ipAddress, const std::string& userName);

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

    /// Get nexilis client API.
    nexilis::client::ClientAPI& getClientAPI()
    {
        return m_clientAPI;
    }

private:
    nexilis::ProtocolManager* m_protocolManager;
    nexilis::client::ClientAPI m_clientAPI;
    nexilis::client::nxboost::TCPClient m_tcpClient;
};

} // namespace nexilis

#endif
