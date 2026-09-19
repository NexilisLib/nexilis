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

namespace Nexilis
{
    public enum ProtocolType
    {
        UNKNOWN = 0,

        AF_INET_UDP_SERVER = 1,
        AF_INET_UDP_CLIENT = 2,
        AF_INET_TCP_SERVER = 3,
        AF_INET_TCP_CLIENT = 4,

        BOOST_UDP_SERVER = 5,
        BOOST_UDP_CLIENT = 6,
        BOOST_TCP_SERVER = 7,
        BOOST_TCP_CLIENT = 8,

        AF_UNIX_SOCK_DGRAM_CLIENT = 9,
        AF_UNIX_SOCK_DGRAM_SERVER = 10,
        AF_UNIX_SOCK_STREAM_CLIENT = 11,
        AF_UNIX_SOCK_STREAM_SERVER = 12,
    }
}
