#ifndef NEXILIS_WEBSOCKET_WEBSOCKET_MACROS_HH
#define NEXILIS_WEBSOCKET_WEBSOCKET_MACROS_HH

#include "../connection.hh"

#include <websocketpp/common/connection_hdl.hpp>
#include <websocketpp/roles/server_endpoint.hpp>
#include <websocketpp/config/asio_no_tls.hpp>

namespace nexilis
{

using websocket = websocketpp::server<websocketpp::config::asio>;
using connection = websocketpp::connection_hdl;
using message = websocket::message_ptr;
using connectionList = std::map<connection, Connection, std::owner_less<connection>>;

}

#endif
