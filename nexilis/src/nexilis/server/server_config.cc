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

#include <nexilis/server/server_config.hh>
#include <nexilis/util.hh>

namespace nexilis::server
{

void ServerConfig::setRootPassword(const std::string& password)
{
    m_rootPassword = password;
}

bool ServerConfig::isRootPassword(const std::string& password)
{
    return Util::constantTimeEquals(m_rootPassword, password);
}

bool ServerConfig::hasRootPassword()
{
    return !m_rootPassword.empty();
}

void ServerConfig::setTickrate(float tickrate)
{
    m_tickrate = tickrate;
}

} // namespace nexilis::server
