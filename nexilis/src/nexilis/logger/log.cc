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

#include <nexilis/logger/console_handler.hh>
#include <nexilis/logger/log.hh>

namespace nexilis
{

logger::Logger Log::log;

void Log::startConsoleLogging(logger::LogLevel minLevel)
{
    log.setMinimumLevel(minLevel);
    log.addHandler(std::make_unique<logger::ConsoleHandler>(logger::ConsoleHandler()));
}

void Log::startConsoleDebugging()
{
    log.setMinimumLevel(logger::LogLevel::Debug);
    log.addHandler(std::make_unique<logger::ConsoleHandler>(logger::ConsoleHandler()));
}

void Log::stopLogging()
{
    log.unsetAllLevels();
    clearHandlers();
}

void Log::startConsoleLogging(uint8_t logLevel)
{
    log.setLogLevel(logLevel);

    log.addHandler(std::make_unique<logger::ConsoleHandler>(logger::ConsoleHandler()));
}

} // namespace nexilis
