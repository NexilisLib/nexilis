#ifndef NEXILIS_CLIENT_PROTOCOL_HH
#define NEXILIS_CLIENT_PROTOCOL_HH

#include <nexilis/client_api.hh>

#include <cstdint>
#include <string>
#include <memory>
#include <vector>

namespace nexilis
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

    /// Send message from client to server.
    /// \param message The string message that is sent.
    virtual void sendMessage(const std::string& message) = 0;

    /// Send nexilis message (std::vector<uint8_t>) to server.
    virtual void sendMessage(const std::vector<uint8_t>& message) = 0;

    ClientAPI* getClientAPI()
    {
        return m_api;
    }

private:
    ClientAPI* m_api;
};

} // namespace nexilis

#endif
