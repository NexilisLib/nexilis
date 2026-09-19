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

#ifndef NEXILIS_SERVER_MESSAGE_ERROR_MESSAGE_HH
#define NEXILIS_SERVER_MESSAGE_ERROR_MESSAGE_HH

#include <nexilis/server/message/base_message.hh>

namespace nexilis::server
{

class ErrorMessage : public BaseMessage
{
public:
    enum class Type
    {
        client_id_failure,
        empty_authentication_mode,
        missing_authentication_mode,
        authentication_error,
        not_implemented,
        no_access
    };

    /// Constructor.
    ErrorMessage(const std::string& address, Type errorType);

    /// Deleted copy constructor.
    ErrorMessage(const ErrorMessage&) = delete;

    /// Deleted copy assignment operator.
    ErrorMessage& operator=(const ErrorMessage&) = delete;

    /// Move constructor.
    ErrorMessage(ErrorMessage&& other) noexcept;

    /// Move assignment operator.
    ErrorMessage& operator=(ErrorMessage&& other) noexcept;

    /// Type info.
    BaseMessage::Type getType() override
    {
        return BaseMessage::Type::error_message;
    }

private:
    Type m_errorType;
};

} // namespace nexilis::server

#endif
