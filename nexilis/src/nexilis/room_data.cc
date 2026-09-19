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

#include <nexilis/room_data.hh>

namespace nexilis
{

RoomData::RoomData(uint64_t creatorId, const std::string& name, uint64_t roomId, Context context, uint32_t maxSize)
    : m_creatorId(creatorId),
      m_name(name),
      m_roomId(roomId),
      m_context(context),
      m_maxSize(maxSize)
{
}

RoomData::RoomData(const RoomData& other)
    : m_creatorId(other.m_creatorId),
      m_name(other.m_name),
      m_roomId(other.m_roomId),
      m_context(other.m_context),
      m_maxSize(other.m_maxSize)
{
}

RoomData::RoomData(RoomData&& other)
    : m_creatorId(std::move(other.m_creatorId)),
      m_name(std::move(other.m_name)),
      m_roomId(std::move(other.m_roomId)),
      m_context(std::move(other.m_context)),
      m_maxSize(std::move(other.m_maxSize))
{
}

RoomData& RoomData::operator=(const RoomData& other)
{
    if (this != &other)
    {
        m_creatorId = other.m_creatorId;
        m_name = other.m_name;
        m_roomId = other.m_roomId;
        m_context = other.m_context;
        m_maxSize = other.m_maxSize;
    }
    return *this;
}

RoomData& RoomData::operator=(RoomData&& other)
{
    if (this != &other)
    {
        m_creatorId = std::move(other.m_creatorId);
        m_name = std::move(other.m_name);
        m_roomId = std::move(other.m_roomId);
        m_context = std::move(other.m_context);
        m_maxSize = std::move(other.m_maxSize);
    }
    return *this;
}

} // namespace nexilis
