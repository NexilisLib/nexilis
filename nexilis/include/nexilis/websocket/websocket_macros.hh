#ifndef NEXILIS_WEBSOCKET_WEBSOCKET_MACROS_HH
#define NEXILIS_WEBSOCKET_WEBSOCKET_MACROS_HH

#include <websocketpp/common/connection_hdl.hpp>
#include <websocketpp/roles/server_endpoint.hpp>
#include <websocketpp/config/asio_no_tls.hpp>


namespace nexilis
{

using wpp_websocket = websocketpp::server<websocketpp::config::asio>;
using wpp_connection = websocketpp::connection_hdl;
using wpp_message = wpp_websocket::message_ptr;

}

#endif
