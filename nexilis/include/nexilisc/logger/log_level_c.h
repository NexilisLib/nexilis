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

#ifndef NEXILISC_LOGGER_LOG_LEVEL_C_H
#define NEXILISC_LOGGER_LOG_LEVEL_C_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    NEXILIS_LOGGER_LOGLEVEL_DEBUG,
    NEXILIS_LOGGER_LOGLEVEL_INFO,
    NEXILIS_LOGGER_LOGLEVEL_WARNING,
    NEXILIS_LOGGER_LOGLEVEL_ERROR,
    NEXILIS_LOGGER_LOGLEVEL_CRITICAL
} nexilis_logger_loglevel;

#ifdef __cplusplus
}
#endif

#endif
