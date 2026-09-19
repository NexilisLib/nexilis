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

#include <nexilis/base_client.hh>

#include <utility>

namespace nexilis
{

BaseClient::BaseClient(uint64_t id)
    : m_id(id),
      m_object2D(id),
      m_object3D(id)
{
}

BaseClient::BaseClient(BaseClient&& other) noexcept
    : m_id(std::move(other.m_id)),
      m_username(std::move(other.m_username)),
      m_object2D(std::move(other.m_object2D)),
      m_object3D(std::move(other.m_object3D))
{
}

BaseClient& BaseClient::operator=(BaseClient&& other) noexcept
{
    if (this != &other)
    {
        m_id = std::move(other.m_id);
        m_username = std::move(other.m_username);
        m_object2D = std::move(other.m_object2D);
        m_object3D = std::move(other.m_object3D);
    }
    return *this;
}

} // namespace nexilis
