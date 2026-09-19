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

#include <nexilisc/logger/console_handler_c.h>

nexilis_logger_ConsoleHandler* nexilis_logger_ConsoleHandler_create()
{
    auto console_handler = new nexilis_logger_ConsoleHandler();
    console_handler->handler = new nexilis::logger::ConsoleHandler();
    return console_handler;
}

void nexilis_logger_ConsoleHandler_destroy(nexilis_logger_ConsoleHandler* handler)
{
    if (handler)
    {
        if (handler->handler)
        {
            delete handler->handler;
            handler->handler = nullptr;
        }
        delete handler;
    }
}

uint64_t nexilis_logger_ConsoleHandler_get_id(nexilis_logger_ConsoleHandler* handler)
{
    if (handler && handler->handler)
    {
        return handler->handler->getId();
    }
    return 0;
}
