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

#ifndef NEXILIS_PROTOCOL_HH
#define NEXILIS_PROTOCOL_HH

#include <string>

namespace nexilis
{

class Protocol
{
public:
    // These are the types inherited from this class.
    enum class Type
    {
        UNKNOWN,

        AF_INET_UDP_SERVER,
        AF_INET_UDP_CLIENT,
        AF_INET_TCP_SERVER,
        AF_INET_TCP_CLIENT,

        BOOST_UDP_SERVER,
        BOOST_UDP_CLIENT,
        BOOST_TCP_SERVER,
        BOOST_TCP_CLIENT,

        AF_UNIX_SOCK_DGRAM_CLIENT,
        AF_UNIX_SOCK_DGRAM_SERVER,
        AF_UNIX_SOCK_STREAM_CLIENT,
        AF_UNIX_SOCK_STREAM_SERVER
    };

    // Enum for server or client protocol.
    enum class Mode
    {
        UNKNOWN,
        SERVER,
        CLIENT
    };

    /// Default constructor.
    Protocol() = default;

    /// Move constructor.
    Protocol(Protocol&& other) = default;

    /// Move assignment operator.
    Protocol& operator=(Protocol&& other) = default;

    /// Deleted copy constructor.
    Protocol(const Protocol& other) = delete;

    /// Deleted copy assignment operator.
    Protocol& operator=(const Protocol& other) = delete;

    /// Virtual destruction.
    virtual ~Protocol() = default;

    /// Start running protocol instance.
    virtual void start() = 0;

    /// Stop running protocol instance.
    virtual void stop() = 0;

    /// Get the associated Protocol::Type from the protocol.
    /// \note New types to Protocol::Type.
    virtual Type getType() = 0;

    /// Get the mode of a type.
    static Mode getMode(Type type);

    /// Returns a string value of the Type.
    static std::string typeToString(Type type);
};

} // namespace nexilis

#endif
