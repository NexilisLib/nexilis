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

#include <cstdint>
#include <nexilis/server/config.hh>

namespace nexilis::server
{

bool Config::m_bigEndian = isSystemBigEndian();

bool Config::isSystemBigEndian()
{
    union
    {
        uint16_t value;
        uint8_t bytes[2];
    } test = {0x0102};
    return test.bytes[0] == 0x01; // True if big-endian
}

} // namespace nexilis::server
