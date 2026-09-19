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

#ifndef NEXILIS_AUTH_CONFIG_HH
#define NEXILIS_AUTH_CONFIG_HH

#include <nexilis/server/authentication_mode.hh>
#include <nexilis/util.hh>

#include <string>

namespace nexilis
{

using server::AuthenticationMode;

class AuthConfig
{
public:
    AuthConfig() = default;
    AuthConfig(AuthConfig&& other);
    AuthConfig& operator=(AuthConfig&& other);
    AuthConfig(const AuthConfig& other);
    AuthConfig& operator=(const AuthConfig& other);

    AuthConfig(AuthenticationMode mode, std::string password)
        : m_mode(mode),
          m_password(std::move(password))
    {
    }

    AuthenticationMode getMode() const
    {
        return m_mode;
    }

    void setMode(AuthenticationMode mode)
    {
        m_mode = mode;
    }

    const std::string& getPassword() const
    {
        return m_password;
    }

    void setPassword(const std::string& password)
    {
        m_password = password;
    }

    bool isPassword(const std::string& password) const
    {
        return Util::constantTimeEquals(m_password, password);
    }

    bool hasPassword() const
    {
        return !m_password.empty();
    }

private:
    AuthenticationMode m_mode = AuthenticationMode::empty;
    std::string m_password;
};

} // namespace nexilis

#endif
