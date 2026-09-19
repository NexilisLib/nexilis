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

#ifndef NEXILISC_CLIENT_PROTOCOL_BOOST_TCP_CLIENT_C_H
#define NEXILISC_CLIENT_PROTOCOL_BOOST_TCP_CLIENT_C_H

#include <nexilisc/protocol_manager_c.h>
#include <nexilisc/client/client_api_c.h>

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct nexilis_BoostTCPClient nexilis_BoostTCPClient;

nexilis_BoostTCPClient* nexilis_boost_tcp_client_create(nexilis_ProtocolManagerC* manager, nexilis_ClientAPI* client_api);
void nexilis_boost_tcp_client_destroy(nexilis_BoostTCPClient* client);
nexilis_ProtocolTypeC nexilis_boost_tcp_client_get_type(nexilis_BoostTCPClient* client);
void nexilis_boost_tcp_client_start(nexilis_BoostTCPClient* client);
void nexilis_boost_tcp_client_stop(nexilis_BoostTCPClient* client);
void nexilis_boost_tcp_client_send_message(nexilis_BoostTCPClient* client, const uint8_t message[], size_t message_size);
void nexilis_boost_tcp_client_send_message_with_callback(nexilis_BoostTCPClient* client, const uint8_t message[], size_t message_size, void (*callback)(const uint8_t*, size_t));
bool nexilis_boost_tcp_client_is_connected(nexilis_BoostTCPClient* client);

/// Runs the full startClient sequence: start, send clientId, wait for init, send rooms info.
/// Returns true on success.
bool nexilis_start_client(nexilis_ClientAPI* client_api, nexilis_BoostTCPClient* tcp_client);

#ifdef __cplusplus
}
#endif

#endif
