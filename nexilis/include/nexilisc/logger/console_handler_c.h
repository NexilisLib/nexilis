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

#ifndef NEXILISC_LOGGER_CONSOLE_HANDLER_C_H
#define NEXILISC_LOGGER_CONSOLE_HANDLER_C_H

#include <nexilis/logger/console_handler.hh>

#ifdef __cplusplus
extern "C" {
#endif

struct nexilis_logger_ConsoleHandler
{
    nexilis::logger::ConsoleHandler* handler;
};

nexilis_logger_ConsoleHandler* nexilis_logger_ConsoleHandler_create();
void nexilis_logger_ConsoleHandler_destroy(nexilis_logger_ConsoleHandler* handler);
uint64_t nexilis_logger_ConsoleHandler_get_id(nexilis_logger_ConsoleHandler* handler);

#ifdef __cplusplus
}
#endif

#endif
