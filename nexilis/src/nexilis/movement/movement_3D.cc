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

#include <nexilis/movement/movement_3D.hh>

namespace nexilis
{

Movement3D::Movement3D(const MovementData& params, Vector3f movement_amount, const MovementFunc& movement_function)
    : Movement<Vector3f>(params),
      m_movementAmount(movement_amount),
      m_movementFunction(movement_function)
{
}

} // namespace nexilis
