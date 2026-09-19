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

#ifndef NEXILIS_MOVEMENT_DATA_HH
#define NEXILIS_MOVEMENT_DATA_HH

#include <nexilis/nx_data.hh>

namespace nexilis
{

class MovementData
{
public:
    /// Constructor.
    explicit MovementData(uint64_t object_id, float delta_time, const nx_data& message_data, uint64_t message_id);

    uint64_t getObjectId() const
    {
        return m_objectId;
    }

    float getDeltatime() const
    {
        return m_deltaTime;
    }

    nx_data getMessageData() const
    {
        return m_messageData;
    }

    uint64_t getMessageId() const
    {
        return m_messageId;
    }

private:
    uint64_t m_objectId;
    float m_deltaTime;
    nx_data m_messageData;
    uint64_t m_messageId;
};

} // namespace nexilis

#endif
