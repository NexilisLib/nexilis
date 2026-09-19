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

#ifndef NEXILIS_SERVER_MESSAGE_AUTH_MESSAGE_HH
#define NEXILIS_SERVER_MESSAGE_AUTH_MESSAGE_HH

#include <nexilis/server/message/base_message.hh>

namespace nexilis::server
{

class AuthMessage : public BaseMessage
{
public:
    /// Constructor.
    AuthMessage(BaseMessage::Data&& baseData, const std::vector<nx_data>& data);

    /// Deleted copy constructor.
    AuthMessage(const AuthMessage&) = delete;

    /// Deleted copy assignment operator.
    AuthMessage& operator=(const AuthMessage&) = delete;

    /// Move constructor.
    AuthMessage(AuthMessage&& other) noexcept;

    /// Move assignment operator.
    AuthMessage& operator=(AuthMessage&& other) noexcept;

    /// Type info.
    BaseMessage::Type getType() override
    {
        return BaseMessage::Type::auth_message;
    }

    const std::vector<nx_data>& getData() const
    {
        return m_data;
    }

private:
    std::vector<nx_data> m_data;
};

} // namespace nexilis::server

#endif
