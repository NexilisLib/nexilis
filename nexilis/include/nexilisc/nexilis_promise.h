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

#ifndef NEXILISC_PROMISE_C_H
#define NEXILISC_PROMISE_C_H

#include <future>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

    struct nexilis_PromiseHandle
    {
        std::promise<void> promise;
    };

    nexilis_PromiseHandle* nexilis_promise_create();
    void nexilis_promise_destroy(nexilis_PromiseHandle* promise);

#ifdef __cplusplus
}
#endif

#endif // NEXILISC_PROMISE_C_H
