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

#ifndef NEXILIS_LOGGER_FILE_LOG_HH
#define NEXILIS_LOGGER_FILE_LOG_HH

#include <nexilis/logger/logger.hh>

namespace nexilis
{

class FileLog
{
public:
    template <typename T, typename... Args>
    static void debug(const T& data, const Args&... args)
    {
        setup();
        m_logger.debug(data, args...);
    }

    template <typename T, typename... Args>
    static void info(const T& data, const Args&... args)
    {
        setup();
        m_logger.info(data, args...);
    }

    template <typename T, typename... Args>
    static void warning(const T& data, const Args&... args)
    {
        setup();
        m_logger.warning(data, args...);
    }

    template <typename T, typename... Args>
    static void error(const T& data, const Args&... args)
    {
        setup();
        m_logger.error(data, args...);
    }

    template <typename T, typename... Args>
    static void critical(const T& data, const Args&... args)
    {
        setup();
        m_logger.critical(data, args...);
    }

private:
    static void setup();
    static bool m_is_setup;

private:
    static logger::Logger m_logger;
};

} // namespace nexilis

#endif
