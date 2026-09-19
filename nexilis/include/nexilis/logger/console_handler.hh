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

#ifndef NEXILIS_LOGGER_CONSOLE_HANDLER_HH
#define NEXILIS_LOGGER_CONSOLE_HANDLER_HH

#include <nexilis/logger/base_handler.hh>

namespace nexilis::logger
{

class ConsoleHandler : public BaseHandler
{
public:
    /// Constructor.
    ConsoleHandler() = default;

    /// Print messages to the console.
    /// \param logLevel The log level of the given message.
    /// \param data The data of the given message.
    void emit(LogLevel logLevel, const std::string& data) override;

    // Overloading the equality operator.
    bool operator==(const BaseHandler& other) const override
    {
        return this == &other;
    }
};

} // namespace nexilis::logger

#endif
