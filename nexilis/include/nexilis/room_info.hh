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

#ifndef NEXILIS_ROOM_INFO_HH
#define NEXILIS_ROOM_INFO_HH

#include <cstdint>
#include <string>

namespace nexilis
{

class RoomInfo
{
public:
    // Constructor
    RoomInfo(uint64_t id, const std::string& name)
        : m_id(id), m_name(name)
    {
    }

    uint64_t getId() const
    {
        return m_id;
    }
    const std::string& getName() const
    {
        return m_name;
    }

private:
    uint64_t m_id;
    std::string m_name;
};

} // namespace nexilis

#endif
