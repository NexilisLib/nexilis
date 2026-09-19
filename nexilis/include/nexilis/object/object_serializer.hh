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

#ifndef NEXILIS_OBJECT_SERIALIZER_HH
#define NEXILIS_OBJECT_SERIALIZER_HH

#include <nexilis/nexilis_constants.hh>

#include <nexilis/object/object_2d.hh>
#include <nexilis/object/object_3d.hh>

namespace nexilis
{

class ObjectSerializer
{
public:
    template <typename T>
    static nx_data data(const std::unique_ptr<Object<T>> object)
    {
        return object.getData();
    }

    virtual Object2D* asObject2D(const nx_data& input_data)
    {
        (void)input_data;
        return nullptr;
    }

    virtual Object3D* asObject3D(const nx_data& input_data)
    {
        (void)input_data;
        return nullptr;
    }
};

} // namespace nexilis

#endif
