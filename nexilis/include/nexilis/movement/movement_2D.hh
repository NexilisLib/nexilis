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

#ifndef NEXILIS_MOVEMENT_2DMOVEMENT_HH
#define NEXILIS_MOVEMENT_2DMOVEMENT_HH

#include <nexilis/movement/movement.hh>

#include <nexilis/types/vector2.hh>

namespace nexilis
{

class Movement2D : public Movement<Vector2f>
{
public:
    /// Constructor.
    explicit Movement2D(const MovementData& params, Vector2f movement_amount, const MovementFunc& movement_function)
        : Movement<Vector2f>(params),
          m_movementAmount(movement_amount),
          m_movementFunction(movement_function)
    {
    }

    /// Movement::getType implementation.
    Type getType() const override
    {
        return Type::_2D;
    }

    /// Movement::getAmount implementation.
    Vector2f getAmount() const override
    {
        return m_movementAmount;
    }

    MovementFunc getMovementFunc() const
    {
        return m_movementFunction;
    }

private:
    Vector2f m_movementAmount;
    MovementFunc m_movementFunction;
};

} // namespace nexilis

#endif
