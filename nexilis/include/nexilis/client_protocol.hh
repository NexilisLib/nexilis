#ifndef NEXILIS_CLIENT_PROTOCOL_HH
#define NEXILIS_CLIENT_PROTOCOL_HH

#include <string>
#include <vector>
#include <cstdint>

namespace nexilis
{

class ClientProtocol
{
public:
    /// Send message from client to server.
    /// \param message The string message that is sent.
    virtual void sendMessage(const std::string& message) = 0;

    /// Send message from client to server.
    /// TODO Pure virtualize.
    virtual void sendMessage(const std::vector<uint8_t>& message)
    {
        (void)message;
    }
};

}

#endif
