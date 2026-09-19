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

#ifndef NEXILISC_LOGGER_FILE_HANDLER_C_H
#define NEXILISC_LOGGER_FILE_HANDLER_C_H

#include <nexilis/logger/file_handler.hh>

#ifdef __cplusplus
extern "C" {
#endif

struct nexilis_logger_FileHandler
{
    nexilis::logger::FileHandler* handler;
};

nexilis_logger_FileHandler* nexilis_logger_FileHandler_create(const char* filename);
void nexilis_logger_FileHandler_destroy(nexilis_logger_FileHandler* handler);
uint64_t nexilis_logger_FileHandler_get_id(nexilis_logger_FileHandler* handler);

#ifdef __cplusplus
}
#endif

#endif
