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

#ifndef NEXILISC_SERVER_AUTHENTICATION_MODE_C_H
#define NEXILISC_SERVER_AUTHENTICATION_MODE_C_H

#ifdef __cplusplus
extern "C" {
#endif

// Enum for AuthenticationMode
typedef enum {
    AUTHENTICATION_MODE_EMPTY,
    AUTHENTICATION_MODE_SKIP,
    AUTHENTICATION_MODE_PASSWORD_PROTECTED,
    AUTHENTICATION_MODE_ADMIN_ACCESS,
    AUTHENTICATION_MODE_ROOT_ACCESS
} nexilis_server_AuthenticationModeC;

#ifdef __cplusplus
}
#endif

#endif
