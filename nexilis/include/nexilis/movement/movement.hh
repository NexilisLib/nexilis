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

#ifndef NEXILIS_MOVEMENT_MOVEMENT_HH
#define NEXILIS_MOVEMENT_MOVEMENT_HH

#include <nexilis/movement/movement_data.hh>
#include <nexilis/nexilis_constants.hh>
#include <nexilis/nx_data.hh>
#include <nexilis/types/vector2.hh>

#include <functional>

namespace nexilis
{

template <typename T>
class Movement
{
public:
    using MovementFunc = std::function<double(double, double)>;

    enum class Type
    {
        _2D,
        _3D
    };

    /// Constructor.
    explicit Movement(const MovementData& data)
        : m_data(data)
    {
    }

    virtual Type getType() const = 0;
    virtual T getAmount() const = 0;

    uint64_t getObjectId() const
    {
        return m_data.getObjectId();
    }

    float getDeltatime() const
    {
        return m_data.getDeltatime();
    }

    nx_data getMessageData() const
    {
        return m_data.getMessageData();
    }

    uint64_t getMessageId() const
    {
        return m_data.getMessageId();
    }

private:
    MovementData m_data;
};

} // namespace nexilis

#endif
