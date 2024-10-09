#ifndef NEXILIS_CLIENT_PROTOCOL_HH
#define NEXILIS_CLIENT_PROTOCOL_HH

#include <nexilis/client/client_api.hh>

namespace nexilis::client
{

class ClientProtocol
{
public:
    /// Constructor.
    ClientProtocol(ClientAPI* api);

    /// Move constructor.
    ClientProtocol(ClientProtocol&& other);

    /// Move assignment operator.
    ClientProtocol& operator=(ClientProtocol&& other);

    /// Deleted copy constructor.
    ClientProtocol(const ClientProtocol& other) = delete;

    /// Deleted copy assignment operator.
    ClientProtocol& operator=(const ClientProtocol& other) = delete;

    /// Send nexilis message (std::vector<uint8_t>) to server.
    virtual void sendMessage(const std::vector<uint8_t>& message) = 0;

    /// Send nexilis message with callback.
    virtual void sendMessage(const std::vector<uint8_t>& message, const std::function<void()>& callback) = 0;

    ClientAPI* getClientAPI()
    {
        return m_api;
    }

    /// Create pair that contains the id of the message and the callback itself.
    std::pair<uint64_t, std::function<void()>> createCallback(const std::vector<uint8_t>& message, const std::function<void()>& callback);

private:
    ClientAPI* m_api;
};

} // namespace nexilis::client

#endif
