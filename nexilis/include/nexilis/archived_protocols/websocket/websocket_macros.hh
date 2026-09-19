/* Copyright (C) 2026 Valtteri Viirret
   This file is part of the Nexilis Project.

   This file is free software: you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as
   published by the Free Software Foundation, either version 3 of the
   License, or (at your option) any later version.

   This file is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with this file.  If not, see <https://gnu.org>. */

#ifndef NEXILIS_WEBSOCKET_WEBSOCKET_MACROS_HH
#define NEXILIS_WEBSOCKET_WEBSOCKET_MACROS_HH

#include <websocketpp/common/connection_hdl.hpp>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/roles/server_endpoint.hpp>

namespace nexilis
{

using wpp_websocket = websocketpp::server<websocketpp::config::asio>;
using wpp_connection = websocketpp::connection_hdl;
using wpp_message = wpp_websocket::message_ptr;

} // namespace nexilis

#endif
