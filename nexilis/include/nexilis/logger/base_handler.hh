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

#ifndef NEXILIS_LOGGER_BASE_HANDLER_HH
#define NEXILIS_LOGGER_BASE_HANDLER_HH

#include <nexilis/logger/log_level.hh>
#include <nexilis/util.hh>

#include <cstdint>
#include <string>

namespace nexilis::logger
{

class BaseHandler
{
public:
    /// Destructor.
    virtual ~BaseHandler() = default;

    // Overloading the equality operator.
    virtual bool operator==(const BaseHandler& other) const = 0;

    // Handle logs.
    // \param logLevel The log level of the message.
    // \param data The data of the given message.
    virtual void emit(LogLevel logLevel, const std::string& data) = 0;

    uint64_t getId() const
    {
        return m_id;
    }

private:
    uint64_t m_id = Util::getRandomUint64();
};

} // namespace nexilis::logger

#endif
