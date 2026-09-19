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

#include <nexilisc/types/vector3_c.h>

#include <cassert>
#include <cstring>
#include <vector>

#include <nexilis/logger/file_log.hh>

template <typename VectorType>
bool nexilis_vector3_is_valid(void* vector_ptr)
{
    if (!vector_ptr)
    {
        return false;
    }

    try
    {
        VectorType* vec = reinterpret_cast<VectorType*>(vector_ptr);

        if (!vec->vec)
        {
            return false;
        }

        using ComponentType = std::remove_reference_t<decltype(vec->vec->x)>;

        volatile auto x = vec->vec->x;
        volatile auto y = vec->vec->y;
        volatile auto z = vec->vec->z;
        (void)x;
        (void)y;
        (void)z;

        if constexpr (std::is_floating_point_v<ComponentType>)
        {
            return std::isfinite(x) &&
                   std::isfinite(y) &&
                   std::isfinite(z);
        }
        else
        {
            return true;
        }
    }
    catch (...)
    {
        return false;
    }
}

// nexilis_Vector3f implementation
nexilis_Vector3f* nexilis_vector3f_create(float x, float y, float z)
{
    try
    {
        auto* vector = new (std::nothrow) nexilis_Vector3f;
        if (!vector)
        {
            return nullptr;
        }
        vector->vec = new nexilis::Vector3f(x, y, z);
        if (!vector->vec)
        {
            delete vector;
            return nullptr;
        }
        return vector;
    }
    catch (...)
    {
        nexilis::FileLog::critical("Vector3f creation failed");
        return nullptr;
    }
}

nexilis_Vector3f* nexilis_vector3f_create_default()
{
    try
    {
        auto* vector = new (std::nothrow) nexilis_Vector3f;
        if (!vector)
        {
            return nullptr;
        }
        vector->vec = new (std::nothrow) nexilis::Vector3f();
        if (!vector->vec)
        {
            delete vector;
            return nullptr;
        }
        return vector;
    }
    catch (...)
    {
        nexilis::FileLog::critical("Vector3f default creation failed");
        return nullptr;
    }
}

void nexilis_vector3f_destroy(nexilis_Vector3f* vec)
{
    if (vec)
    {
        if (vec->vec)
        {
            delete vec->vec;
        }
        delete vec;
    }
}

float nexilis_vector3f_get_x(const nexilis_Vector3f* vec)
{
    return vec->vec->x;
}

float nexilis_vector3f_get_y(const nexilis_Vector3f* vec)
{
    return vec->vec->y;
}

float nexilis_vector3f_get_z(const nexilis_Vector3f* vec)
{
    return vec->vec->z;
}

void nexilis_vector3f_set_x(nexilis_Vector3f* vec, float x)
{
    vec->vec->x = x;
}

void nexilis_vector3f_set_y(nexilis_Vector3f* vec, float y)
{
    vec->vec->y = y;
}

void nexilis_vector3f_set_z(nexilis_Vector3f* vec, float z)
{
    vec->vec->z = z;
}

void nexilis_vector3f_serialize(const nexilis_Vector3f* vec, uint8_t* out_data)
{
    auto data = vec->vec->serialize();
    memcpy(out_data, data.data(), data.size());
}

nexilis_Vector3f* nexilis_vector3f_deserialize(const uint8_t* data)
{
    const size_t expected_size = sizeof(float) * 3;
    if (!data)
        return nullptr;

    nexilis::nx_data converted_data(data, data + expected_size);
    try
    {
        auto* deserialized = new nexilis::Vector3f(nexilis::Vector3<float>::deserialize(converted_data));
        auto* new_vector = new nexilis_Vector3f;
        new_vector->vec = deserialized;
        return new_vector;
    }
    catch (...)
    {
        return nullptr;
    }
}

bool nexilis_vector3f_is_valid(void* vector_ptr)
{
    return nexilis_vector3_is_valid<nexilis_Vector3f>(vector_ptr);
}

// nexilis_Vector3u implementation
nexilis_Vector3u* nexilis_vector3u_create(uint64_t x, uint64_t y, uint64_t z)
{
    try
    {
        auto* vector = new (std::nothrow) nexilis_Vector3u;
        if (!vector)
        {
            return nullptr;
        }
        vector->vec = new nexilis::Vector3u(x, y, z);
        if (!vector->vec)
        {
            delete vector;
            return nullptr;
        }
        return vector;
    }
    catch (...)
    {
        nexilis::FileLog::critical("Vector3u creation failed");
        return nullptr;
    }
}

nexilis_Vector3u* nexilis_vector3u_create_default()
{
    try
    {
        auto* vector = new (std::nothrow) nexilis_Vector3u;
        if (!vector)
        {
            return nullptr;
        }
        vector->vec = new (std::nothrow) nexilis::Vector3u();
        if (!vector->vec)
        {
            delete vector;
            return nullptr;
        }
        return vector;
    }
    catch (...)
    {
        nexilis::FileLog::critical("Vector3u default creation failed");
        return nullptr;
    }
}

void nexilis_vector3u_destroy(nexilis_Vector3u* vec)
{
    if (vec)
    {
        if (vec->vec)
        {
            delete vec->vec;
        }
        delete vec;
    }
}

uint64_t nexilis_vector3u_get_x(const nexilis_Vector3u* vec)
{
    return vec->vec->x;
}

uint64_t nexilis_vector3u_get_y(const nexilis_Vector3u* vec)
{
    return vec->vec->y;
}

uint64_t nexilis_vector3u_get_z(const nexilis_Vector3u* vec)
{
    return vec->vec->z;
}

void nexilis_vector3u_set_x(nexilis_Vector3u* vec, uint64_t x)
{
    vec->vec->x = x;
}

void nexilis_vector3u_set_y(nexilis_Vector3u* vec, uint64_t y)
{
    vec->vec->y = y;
}

void nexilis_vector3u_set_z(nexilis_Vector3u* vec, uint64_t z)
{
    vec->vec->z = z;
}

void nexilis_vector3u_serialize(const nexilis_Vector3u* vec, uint8_t* out_data)
{
    auto data = vec->vec->serialize();
    memcpy(out_data, data.data(), data.size());
}

nexilis_Vector3u* nexilis_vector3u_deserialize(const uint8_t* data)
{
    const size_t expected_size = sizeof(uint32_t) * 3;
    if (!data)
        return nullptr;

    nexilis::nx_data converted_data(data, data + expected_size);
    try
    {
        auto* deserialized = new nexilis::Vector3u(nexilis::Vector3<uint64_t>::deserialize(converted_data));
        auto* new_vector = new nexilis_Vector3u;
        new_vector->vec = deserialized;
        return new_vector;
    }
    catch (...)
    {
        return nullptr;
    }
}

bool nexilis_vector3u_is_valid(void* vector_ptr)
{
    return nexilis_vector3_is_valid<nexilis_Vector3u>(vector_ptr);
}

// nexilis_Vector3i implementation
nexilis_Vector3i* nexilis_vector3i_create(int x, int y, int z)
{
    try
    {
        auto* vector = new (std::nothrow) nexilis_Vector3i;
        if (!vector)
        {
            return nullptr;
        }
        vector->vec = new nexilis::Vector3i(x, y, z);
        if (!vector->vec)
        {
            delete vector;
            return nullptr;
        }
        return vector;
    }
    catch (...)
    {
        nexilis::FileLog::critical("Vector3i creation failed");
        return nullptr;
    }
}

nexilis_Vector3i* nexilis_vector3i_create_default()
{
    try
    {
        auto* vector = new (std::nothrow) nexilis_Vector3i;
        if (!vector)
        {
            return nullptr;
        }
        vector->vec = new (std::nothrow) nexilis::Vector3i();
        if (!vector->vec)
        {
            delete vector;
            return nullptr;
        }
        return vector;
    }
    catch (...)
    {
        nexilis::FileLog::critical("Vector3i default creation failed");
        return nullptr;
    }
}

void nexilis_vector3i_destroy(nexilis_Vector3i* vec)
{
    if (vec)
    {
        if (vec->vec)
        {
            delete vec->vec;
        }
        delete vec;
    }
}

int nexilis_vector3i_get_x(const nexilis_Vector3i* vec)
{
    return vec->vec->x;
}

int nexilis_vector3i_get_y(const nexilis_Vector3i* vec)
{
    return vec->vec->y;
}

int nexilis_vector3i_get_z(const nexilis_Vector3i* vec)
{
    return vec->vec->z;
}

void nexilis_vector3i_set_x(nexilis_Vector3i* vec, int x)
{
    vec->vec->x = x;
}

void nexilis_vector3i_set_y(nexilis_Vector3i* vec, int y)
{
    vec->vec->y = y;
}

void nexilis_vector3i_set_z(nexilis_Vector3i* vec, int z)
{
    vec->vec->z = z;
}

void nexilis_vector3i_serialize(const nexilis_Vector3i* vec, uint8_t* out_data)
{
    auto data = vec->vec->serialize();
    memcpy(out_data, data.data(), data.size());
}

nexilis_Vector3i* nexilis_vector3i_deserialize(const uint8_t* data)
{
    const size_t expected_size = sizeof(uint32_t) * 3;
    if (!data)
        return nullptr;

    nexilis::nx_data converted_data(data, data + expected_size);
    try
    {
        auto* deserialized = new nexilis::Vector3i(nexilis::Vector3<int>::deserialize(converted_data));
        auto* new_vector = new nexilis_Vector3i;
        new_vector->vec = deserialized;
        return new_vector;
    }
    catch (...)
    {
        return nullptr;
    }
}

bool nexilis_vector3i_is_valid(void* vector_ptr)
{
    return nexilis_vector3_is_valid<nexilis_Vector3i>(vector_ptr);
}
