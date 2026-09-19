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

#include <nexilisc/nx_data_c.h>

#include <cstdlib>
#include <cstring>

nx_data_c nexilis_nx_data_create(uint64_t size)
{
    nx_data_c result;
    result.data = new nexilis::nx_data(size);
    return result;
}

nx_data_c nexilis_nx_data_create_from(const uint8_t* data, uint64_t size)
{
    nx_data_c result;
    auto* vec = new nexilis::nx_data(size);
    if (data && size > 0)
    {
        memcpy(vec->data(), data, size);
    }
    result.data = vec;
    return result;
}

void nexilis_nx_data_destroy(nx_data_c* data)
{
    if (data && data->data)
    {
        delete data->data;
        data->data = nullptr;
    }
}

uint64_t nexilis_nx_data_get_size(const nx_data_c* data)
{
    if (!data || !data->data)
    {
        return 0;
    }
    return data->data->size();
}

const uint8_t* nexilis_nx_data_get_data(const nx_data_c* data)
{
    if (!data || !data->data)
    {
        return nullptr;
    }
    auto* vec = static_cast<nexilis::nx_data*>(data->data);
    return vec->empty() ? nullptr : vec->data();
}

uint8_t* nexilis_nx_data_get_mutable(nx_data_c* data)
{
    if (!data || !data->data)
    {
        return nullptr;
    }
    auto* vec = static_cast<nexilis::nx_data*>(data->data);
    return vec->empty() ? nullptr : vec->data();
}
