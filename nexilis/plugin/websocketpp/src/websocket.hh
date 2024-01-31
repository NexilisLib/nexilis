#ifndef NEXILIS_WEBSOCKET_WEBSOCKET_HH
#define NEXILIS_WEBSOCKET_WEBSOCKET_HH

#include "../src/websocket_macros.hh"

#include <nexilis/ports.hh>
#include <nexilis/protocol.hh>

#include <functional>

/// At some we need global debug.
/// What the fuck?
#define WEBSOCKET_DEBUG true

namespace nexilis
{

// This class acts as a abtraction for the websocketpp library.
class Websocket : public Protocol
{
public:
    /// Constructor.
    /// \param port The port where to set the websocket server.
    Websocket(unsigned port = static_cast<unsigned>(Port::Websocket));

    // Set custom functionality when opening connection.
    void setOpenHandler(const std::function<void()>& openHandler);

    // Set custom functionality when closing connection.
    void setCloseHandler(const std::function<void()>& closeHandler);

    // Start the Websocket server.
    void start() override;

    // Protocol::stop() implementation.
    void stop() override
    {
    }

    // Protocol::getType() implementation.
    Type getType() override
    {
        return Type::Websocket;
    }

private:
    /// Get command as vectors of bytes.
    /// \param msg Message gotten from websocketpp.
    /// \return The command of nexilis.
    std::vector<unsigned char> convertToNexilisCommand(const wpp_message& msg);

    wpp_websocket m_websocket;
};

} // namespace nexilis

#endif
