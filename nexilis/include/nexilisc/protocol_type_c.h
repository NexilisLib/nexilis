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

#ifndef NEXILISC_PROTOCOL_TYPE_C_H
#define NEXILISC_PROTOCOL_TYPE_C_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    PROTOCOL_TYPE_UNKNOWN,

    PROTOCOL_TYPE_AF_INET_UDP_SERVER,
    PROTOCOL_TYPE_AF_INET_UDP_CLIENT,
    PROTOCOL_TYPE_AF_INET_TCP_SERVER,
    PROTOCOL_TYPE_AF_INET_TCP_CLIENT,

    PROTOCOL_TYPE_BOOST_UDP_SERVER,
    PROTOCOL_TYPE_BOOST_UDP_CLIENT,
    PROTOCOL_TYPE_BOOST_TCP_SERVER,
    PROTOCOL_TYPE_BOOST_TCP_CLIENT,

    PROTOCOL_TYPE_AF_UNIX_SOCK_DGRAM_CLIENT,
    PROTOCOL_TYPE_AF_UNIX_SOCK_DGRAM_SERVER,
    PROTOCOL_TYPE_AF_UNIX_SOCK_STREAM_CLIENT,
    PROTOCOL_TYPE_AF_UNIX_SOCK_STREAM_SERVER
} nexilis_ProtocolTypeC;

#ifdef __cplusplus
}
#endif

#endif
