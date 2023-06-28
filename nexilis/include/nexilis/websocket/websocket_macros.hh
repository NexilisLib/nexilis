#ifndef NEXILIS_WEBSOCKET_WEBSOCKET_MACROS_HH
#define NEXILIS_WEBSOCKET_WEBSOCKET_MACROS_HH

#include <websocketpp/server.hpp>
#include <websocketpp/config/asio_no_tls.hpp>

namespace nexilis
{

// Forward declarations.
class WebsocketConnection;

using websocket = websocketpp::server<websocketpp::config::asio>;
using connection = websocketpp::connection_hdl;
using message = websocket::message_ptr;
using connectionList = std::map<connection, WebsocketConnection, std::owner_less<connection>>;

}

#endif
