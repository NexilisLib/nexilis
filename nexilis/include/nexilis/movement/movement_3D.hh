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

#ifndef NEXILIS_MOVEMENT_MOVEMENT_3D_HH
#define NEXILIS_MOVEMENT_MOVEMENT_3D_HH

#include <nexilis/movement/movement.hh>
#include <nexilis/types/vector3.hh>

namespace nexilis
{

class Movement3D : public Movement<Vector3f>
{
public:
    /// Constructor.
    explicit Movement3D(const MovementData& params, Vector3f movement_amount, const MovementFunc& movement_function);

    /// Movement::getType implementation.
    Type getType() const override
    {
        return Type::_3D;
    }

    /// Movement::getAmount implementation.
    Vector3f getAmount() const override
    {
        return m_movementAmount;
    }

    MovementFunc getMovementFunc() const
    {
        return m_movementFunction;
    }

private:
    Vector3f m_movementAmount;
    MovementFunc m_movementFunction;
};

} // namespace nexilis

#endif
