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

#include <nexilis/movement/movement_data.hh>

namespace nexilis
{

MovementData::MovementData(uint64_t object_id, float delta_time, const nx_data& message_data, uint64_t message_id)
    : m_objectId(object_id),
      m_deltaTime(delta_time),
      m_messageData(message_data),
      m_messageId(message_id)
{
}

} // namespace nexilis
