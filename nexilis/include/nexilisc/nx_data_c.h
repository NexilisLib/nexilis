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

#ifndef NEXILISC_NX_DATA_C_H
#define NEXILISC_NX_DATA_C_H

#include <nexilis/nx_data.hh>

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

    struct nx_data_c
    {
        nexilis::nx_data* data;
    };

    nx_data_c nexilis_nx_data_create(uint64_t size);
    nx_data_c nexilis_nx_data_create_from(const uint8_t* data, uint64_t size);
    void nexilis_nx_data_destroy(nx_data_c* data);
    uint64_t nexilis_nx_data_get_size(const nx_data_c* data);
    const uint8_t* nexilis_nx_data_get_data(const nx_data_c* data);
    uint8_t* nexilis_nx_data_get_mutable(nx_data_c* data);

#ifdef __cplusplus
}
#endif

#endif
