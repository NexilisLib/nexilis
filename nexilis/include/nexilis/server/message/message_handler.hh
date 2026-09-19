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

#ifndef NEXILIS_MESSAGE_HANDLER_HH
#define NEXILIS_MESSAGE_HANDLER_HH

#include <nexilis/nx_class.hh>
#include <nexilis/server/server_config.hh>

#include <nexilis/server/message/message.hh>

namespace nexilis::server
{

/// MessageHandler
/// This class receives a server message from server `Protocol`.
/// and returns a Message object back to the protocol.
///
/// `Protocol` then sends the `MessageHandler::Message` to `Command` for parsing.
///
/// `Message` holds the nexilis bytevector containing pure command data.

class MessageHandler : public NxClass
{
public:
    /// Default constructor.
    MessageHandler();

    /// Read the unifiltered server message and return it ready for `Command`.
    /// \param address The incoming message sender address.
    /// \param message The incoming message data.
    /// \param authentication The server authentication levels.
    std::unique_ptr<BaseMessage> readMessage(std::string address, const nx_data& payload, ServerConfig* authentication);

private:
    Message handlePayload(const nx_data& payload, User* user, const std::string& address);
};

} // namespace nexilis::server

#endif
