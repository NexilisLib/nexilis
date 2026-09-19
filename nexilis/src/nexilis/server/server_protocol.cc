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

#include <nexilis/server/server_protocol.hh>

namespace nexilis::server
{

ServerProtocol::ServerProtocol(const ServerConfig& settings)
    : m_command(settings)
{
}

ServerProtocol::ServerProtocol(ServerProtocol&& other)
    : m_messageHandler(std::move(other.m_messageHandler)),
      m_command(std::move(other.m_command)),
      m_activeConnections(other.m_activeConnections.load())
{
}

ServerProtocol& ServerProtocol::operator=(ServerProtocol&& other)
{
    if (this != &other)
    {
        m_messageHandler = std::move(other.m_messageHandler);
        m_command = std::move(other.m_command);
        m_activeConnections = std::move(other.m_activeConnections.load());
    }
    return *this;
}

} // namespace nexilis::server
