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

#ifndef NEXILIS_WEBSOCKET_WEBSOCKET_HH
#define NEXILIS_WEBSOCKET_WEBSOCKET_HH

#include "../src/websocket_macros.hh"

#include <nexilis/ports.hh>
#include <nexilis/protocol.hh>

#include <functional>

/// At some we need global debug.
/// What the fuck?
#define WEBSOCKET_DEBUG true

namespace nexilis
{

// This class acts as a abtraction for the websocketpp library.
class Websocket : public Protocol
{
public:
    /// Constructor.
    /// \param port The port where to set the websocket server.
    Websocket(unsigned port = static_cast<unsigned>(Port::Websocket));

    // Set custom functionality when opening connection.
    void setOpenHandler(const std::function<void()>& openHandler);

    // Set custom functionality when closing connection.
    void setCloseHandler(const std::function<void()>& closeHandler);

    // Start the Websocket server.
    void start() override;

    // Protocol::stop() implementation.
    void stop() override
    {
    }

    // Protocol::getType() implementation.
    Type getType() override
    {
        // return Type::Websocket;
    }

private:
    /// Get command as vectors of bytes.
    /// \param msg Message gotten from websocketpp.
    /// \return The command of nexilis.
    std::vector<unsigned char> convertToNexilisCommand(const wpp_message& msg);

    wpp_websocket m_websocket;

    unsigned m_portNumber;
};

} // namespace nexilis

#endif
