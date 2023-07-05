#ifndef NEXILIS_WEBSOCKET_WEBSOCKET_MACROS_HH
#define NEXILIS_WEBSOCKET_WEBSOCKET_MACROS_HH

#include "../connection.hh"

#include <websocketpp/common/connection_hdl.hpp>
#include <websocketpp/roles/server_endpoint.hpp>
#include <websocketpp/config/asio_no_tls.hpp>

namespace nexilis
{

using wpp_websocket = websocketpp::server<websocketpp::config::asio>;
using wpp_connection = websocketpp::connection_hdl;
using wpp_message = wpp_websocket::message_ptr;
using wpp_connectionList = std::map<wpp_connection, Connection, std::owner_less<wpp_connection>>;

}

#endif
