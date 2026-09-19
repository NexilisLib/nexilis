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

#include <nexilisc/logger/function_handler_c.h>

nexilis_logger_FunctionHandler* nexilis_logger_FunctionHandler_create(void (*handler)(const nexilis::logger::LogLevel&, const char*))
{
    auto function_handler = new nexilis_logger_FunctionHandler();

    auto func_handler = [handler](const nexilis::logger::LogLevel& level, const std::string& message)
    {
        handler(level, message.c_str());
    };
    function_handler->handler = new nexilis::logger::FunctionHandler(func_handler);
    return function_handler;
}

void nexilis_logger_FunctionHandler_destroy(nexilis_logger_FunctionHandler* handler)
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

uint64_t nexilis_logger_FunctionHandler_get_id(nexilis_logger_FunctionHandler* handler)
{
    if (handler && handler->handler)
    {
        return handler->handler->getId();
    }
    return 0;
}
