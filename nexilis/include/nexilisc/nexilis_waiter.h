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

#ifndef NEXILISC_NEXILIS_WAITER_H
#define NEXILISC_NEXILIS_WAITER_H

#include <nexilisc/nexilis_promise.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct nexilis_ClientAPI nexilis_ClientAPI;
    typedef struct nexilis_PromiseHandle nexilis_PromiseHandle;
    typedef struct nexilis_WaiterHandle nexilis_WaiterHandle;

    typedef enum
    {
        NEXILIS_WAIT_UNTIL_ROOMS_CREATED
    } nexilis_WaitType;

    nexilis_WaiterHandle* nexilis_waiter_create(nexilis_ClientAPI* client_api, nexilis_PromiseHandle* promise, nexilis_WaitType wait_type);
    nexilis_WaiterHandle* nexilis_waiter_create_for_rooms_created(nexilis_ClientAPI* client_api, nexilis_PromiseHandle* promise);

    void nexilis_waiter_wait(nexilis_WaiterHandle* waiter);
    void nexilis_waiter_destroy(nexilis_WaiterHandle* waiter);
    bool nexilis_waiter_is_valid(const nexilis_WaiterHandle* waiter);

#ifdef __cplusplus
}
#endif

#endif // NEXILISC_NEXILIS_WAITER_H
