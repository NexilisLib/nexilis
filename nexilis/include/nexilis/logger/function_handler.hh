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

#ifndef NEXILIS_LOGGER_FUNCTION_HANDLER_HH
#define NEXILIS_LOGGER_FUNCTION_HANDLER_HH

#include <nexilis/logger/base_handler.hh>

#include <functional>

namespace nexilis::logger
{

class FunctionHandler : public BaseHandler
{
public:
    /// Constructor.
    explicit FunctionHandler(const std::function<void(LogLevel, const std::string&)>& function)
        : m_function(function)
    {
    }

    void emit(LogLevel logLevel, const std::string& data) override
    {
        m_function(logLevel, data);
    }

    bool operator==(const BaseHandler& other) const override
    {
        return this == &other;
    }

private:
    std::function<void(LogLevel, const std::string&)> m_function;
};

} // namespace nexilis::logger

#endif
