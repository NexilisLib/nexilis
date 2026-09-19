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

#ifndef NEXILIS_SERVER_MESSAGE_MESSAGE_HH
#define NEXILIS_SERVER_MESSAGE_MESSAGE_HH

#include <nexilis/server/message/base_message.hh>

namespace nexilis::server
{

class Message : public BaseMessage
{
public:
    /// Constructor.
    Message(BaseMessage::Data&& baseData, const nx_data& data);

    /// Deleted copy constructor.
    Message(const Message&) = delete;

    /// Deleted copy assignment operator.
    Message& operator=(const Message&) = delete;

    /// Move constructor.
    Message(Message&& other) noexcept;

    /// Move assignment operator.
    Message& operator=(Message&& other) noexcept;

    /// Type info.
    BaseMessage::Type getType() override
    {
        return BaseMessage::Type::message;
    }

    /// Get the message data.
    nx_data getData() const;

private:
    nx_data m_data;
};

} // namespace nexilis::server

#endif
