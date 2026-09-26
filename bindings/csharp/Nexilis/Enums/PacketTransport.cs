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
    /// <summary>
    /// The transport a raw package is sent over.
    /// </summary>
    public enum PacketTransport
    {
        /// <summary>
        /// Reliable, ordered, connection based. Use for room and player
        /// management packets and for anything that must not get lost.
        /// </summary>
        Tcp,

        /// <summary>
        /// Unreliable, unordered, datagram based. Use for latency sensitive
        /// data such as positions, where a dropped packet is cheaper than a
        /// retransmit.
        /// </summary>
        Udp
    }
}
