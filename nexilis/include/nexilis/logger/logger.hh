#ifndef NEXILIS_LOGGER_LOGGER_HH
#define NEXILIS_LOGGER_LOGGER_HH

#include <nexilis/logger/base_handler.hh>
#include <nexilis/logger/log_level.hh>

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <memory>
#include <mutex>
#include <sstream>
#include <vector>

namespace nexilis::logger
{

class Logger
{
public:
    /// Constructor.
    Logger() = default;

    // Add handler to the vector of handlers.
    /// \param handler R-value reference of the handler.
    /// \tparam T The type of handler.
    template <typename T>
    void addHandler(T&& handler)
    {
        std::lock_guard<std::mutex> lock(m_mtx);
        m_handlers.emplace_back(std::make_unique<std::remove_reference_t<T>>(std::forward<T>(handler)));
    }

    template <typename T>
    void removeHandler(T&& handlerToRemove)
    {
        std::lock_guard<std::mutex> lock(m_mtx);

        auto it = std::remove_if(m_handlers.begin(), m_handlers.end(),
            [&](const std::unique_ptr<BaseHandler>& handler)
            { return *handler == handlerToRemove; });

        m_handlers.erase(it, m_handlers.end());
    }

    /// Remove all handlers.
    void clearHandlers()
    {
        m_handlers.clear();
    }

    /// Check if there are no handlers for the logger.
    /// \return True if there are no handlers.
    bool noHandlers()
    {
        return m_handlers.empty();
    }

    /// Send debug message.
    template <typename T, typename... Args>
    void debug(const T& data, const Args&... args)
    {
        concatAndEmit(LogLevel::DEBUG, data, args...);
    }

    /// Send info message.
    template <typename T, typename... Args>
    void info(const T& data, const Args&... args)
    {
        concatAndEmit(LogLevel::INFO, data, args...);
    }

    /// Send warning message.
    template <typename T, typename... Args>
    void warning(const T& data, const Args&... args)
    {
        concatAndEmit(LogLevel::WARNING, data, args...);
    }

    /// Send error message.
    template <typename T, typename... Args>
    void error(const T& data, const Args&... args)
    {
        concatAndEmit(LogLevel::ERROR, data, args...);
    }

    /// Send critical message.
    template <typename T, typename... Args>
    void critical(const T& data, const Args&... args)
    {
        concatAndEmit(LogLevel::CRITICAL, data, args...);
    }

    /// Unset specific log level.
    /// \param level The log level to be unset.
    /// \return If unsetting is successfull.
    bool unsetLevel(const LogLevel& level)
    {
        return (m_logLevel &= static_cast<uint8_t>(~(1 << static_cast<uint8_t>(level))));
    }

    /// Set specific log level.
    /// \param level The log level to be set up.
    /// \return If setting is successfull.
    bool setLevel(const LogLevel& level)
    {
        return (m_logLevel |= (1 << static_cast<uint8_t>(level)));
    }

    /// Check if the given level is in use.
    /// \param level The level to be checked.
    /// \return True if level is in use, false if not.
    bool getLevel(const LogLevel& level)
    {
        return (m_logLevel >> static_cast<uint8_t>(level)) & 1;
    }

    /// Set the minimun operation level for the logger.
    /// The level to be set and all the levels after that will be turned on.
    /// The order of levels can be checked from log_level.hh.
    /// \param level The minimun level that is turned on.
    bool setMinimumLevel(const LogLevel& level)
    {
        return (m_logLevel |= (static_cast<uint8_t>(-1) << static_cast<uint8_t>(level)));
    }

    /// Set the logLevel with custom byte.
    /// \param logLevel The byte that determines
    void setLogLevel(uint8_t logLevel)
    {
        assert((std::is_same<decltype(logLevel), uint8_t>::value));
        m_logLevel = logLevel;
    }

private:
    template <typename T>
    void addToStringStream(std::stringstream& ss, const T& data)
    {
        ss << data;
    }

    template <typename T, typename... Args>
    void concatAndEmit(const LogLevel& logLevel, const T& data, const Args&... args)
    {
        // Return if loglevel is not on.
        if (!getLevel(logLevel))
            return;

        // Concat arguments.
        std::stringstream ss;
        ss << data;

        int unused[] = {0, (addToStringStream(ss, args), 0)...};

        // Silence warning about unused variables.
        (void)unused;

        emitLog(logLevel, ss.str());
    }

    template <typename T>
    void emitLog(const LogLevel& logLevel, const T& data)
    {
        // logLevel to string
        std::string logLevelStr;
        switch (logLevel)
        {
            case LogLevel::DEBUG:
                logLevelStr = "DEBUG: ";
                break;
            case LogLevel::INFO:
                logLevelStr = "INFO: ";
                break;
            case LogLevel::WARNING:
                logLevelStr = "WARNING: ";
                break;
            case LogLevel::ERROR:
                logLevelStr = "ERROR: ";
                break;
            case LogLevel::CRITICAL:
                logLevelStr = "CRITICAL: ";
                break;
        }

        // Create final string to be logged.
        std::ostringstream ss;
        ss << logLevelStr << data;

        std::unique_lock<std::mutex> lock(m_mtx);

        // emit message to all handlers
        for (auto& handler : m_handlers)
            handler->emit(logLevel, ss.str());
    }

private:
    /// Handlers for outputting the message.
    std::vector<std::unique_ptr<BaseHandler>> m_handlers;

    /// Disable all loglevels by default.
    uint8_t m_logLevel = 0;

    std::mutex m_mtx;
};

} // namespace nexilis

#endif
