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

#ifndef NEXILIS_LOGGER_LOGGER_HH
#define NEXILIS_LOGGER_LOGGER_HH

#include <nexilis/logger/base_handler.hh>
#include <nexilis/logger/log_level.hh>

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <memory>
#include <mutex>
#include <sstream>
#include <vector>

namespace nexilis::logger
{

class Logger
{
public:
    /// Default constructor.
    Logger()
        : m_mtx()
    {
    }

    ~Logger() = default;

    // Move constructor.
    Logger(Logger&& other) = delete;

    // Move assignment operator.
    Logger& operator=(Logger&& other) = delete;

    /// Deleted copy constructor.
    Logger(const Logger&) = delete;

    /// Deleted move assignment operator.
    Logger& operator=(const Logger&) = delete;

    // Add handler to the vector of handlers.
    /// \param handler R-value reference of the handler.
    /// \tparam T The type of handler.
    template <typename T>
    void addHandler(std::unique_ptr<T> handler)
    {
        static_assert(std::is_base_of_v<BaseHandler, T>, "T must be derived from BaseHandler");
        std::lock_guard<std::mutex> lock(m_mtx);
        m_handlers.emplace_back(std::move(handler));
    }

    /// Remove handler based on it's identifier.
    void removeHandler(uint64_t handlerId);

    /// Remove all handlers.
    void clearHandlers();

    /// Check if there are no handlers for the logger.
    /// \return True if there are no handlers.
    bool noHandlers();

    /// Send debug message.
    template <typename T, typename... Args>
    void debug(const T& data, const Args&... args)
    {
        concatAndEmit(LogLevel::Debug, data, args...);
    }

    /// Send info message.
    template <typename T, typename... Args>
    void info(const T& data, const Args&... args)
    {
        concatAndEmit(LogLevel::Info, data, args...);
    }

    /// Send warning message.
    template <typename T, typename... Args>
    void warning(const T& data, const Args&... args)
    {
        concatAndEmit(LogLevel::Warning, data, args...);
    }

    /// Send error message.
    template <typename T, typename... Args>
    void error(const T& data, const Args&... args)
    {
        concatAndEmit(LogLevel::Error, data, args...);
    }

    /// Send critical message.
    template <typename T, typename... Args>
    void critical(const T& data, const Args&... args)
    {
        concatAndEmit(LogLevel::Critical, data, args...);
    }

    /// Unset specific log level.
    /// \param level The log level to be unset.
    /// \return True, if unsetting is successfull.
    bool unsetLevel(const LogLevel& level);

    /// Unset all log levels.
    void unsetAllLevels();

    /// Set specific log level.
    /// \param level The log level to be set up.
    /// \return True, if setting is successfull.
    bool setLevel(const LogLevel& level);

    /// Set all log levels.
    void setAllLevels();

    /// Check if the given level is in use.
    /// \param level The level to be checked.
    /// \return True if level is in use, false if not.
    bool getLevel(const LogLevel& level);

    /// Set the minimun operation level for the logger.
    /// The level to be set and all the levels after that will be turned on.
    /// The order of levels can be checked from log_level.hh.
    /// \param level The minimun level that is turned on.
    bool setMinimumLevel(const LogLevel& level);

    /// Set the logLevel with custom byte.
    /// \param logLevel The byte that determines
    void setLogLevel(uint8_t logLevel);

private:
    template <typename T, typename... Args>
    void concatAndEmit(const LogLevel& logLevel, const T& data, const Args&... args)
    {
        // Return if loglevel is not on.
        if (!getLevel(logLevel))
        {
            return;
        }

        // Concat arguments.
        std::stringstream ss;

        auto processArg = [&ss](const auto& arg)
        {
            using ArgType = std::decay_t<decltype(arg)>;

            if constexpr (std::is_same_v<ArgType, nx_data>)
            {
                ss << std::string_view(Util::convertToString(arg));
            }
            else if constexpr (std::is_convertible_v<ArgType, std::string_view>)
            {
                // For string-like types that can be converted to string_view.
                ss << std::string_view(arg);
            }
            else
            {
                // For all other types.
                ss << arg;
            }
        };

        // Process the first argument.
        processArg(data);

        // Process the remaining arguments.
        (processArg(args), ...);

        emitLog(logLevel, ss.str());
    }

    template <typename T>
    void emitLog(const LogLevel& logLevel, const T& data)
    {
        // logLevel to string
        std::string logLevelStr;
        switch (logLevel)
        {
            case LogLevel::Debug:
                logLevelStr = "DEBUG: ";
                break;
            case LogLevel::Info:
                logLevelStr = "INFO: ";
                break;
            case LogLevel::Warning:
                logLevelStr = "WARNING: ";
                break;
            case LogLevel::Error:
                logLevelStr = "ERROR: ";
                break;
            case LogLevel::Critical:
                logLevelStr = "CRITICAL: ";
                break;
        }

        // Create final string to be logged.
        std::ostringstream ss;
        ss << logLevelStr << data;

        std::lock_guard<std::mutex> lock(m_mtx);

        // emit message to all handlers
        for (auto& handler : m_handlers)
        {
            if (handler)
            {
                handler->emit(logLevel, ss.str());
            }
        }
    }

private:
    /// Handlers for outputting the message.
    std::vector<std::unique_ptr<BaseHandler>> m_handlers;

    /// Disable all loglevels by default.
    uint8_t m_logLevel = 0;

    std::mutex m_mtx;
};

} // namespace nexilis::logger

#endif
