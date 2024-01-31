#ifndef NEXILIS_LOG_HH
#define NEXILIS_LOG_HH

#include <nexilis/logger/log_level.hh>
#include <nexilis/logger/logger.hh>

namespace nexilis
{

/// This class hold static instance logging library.
class Log
{
public:
    /// Start static console logging session.
    /// \param minLevel The minimum logging level, see logLevel.hh.
    static void startConsoleLogging(logger::LogLevel minLevel = logger::LogLevel::INFO);

    /// Custom loglevel can be set with first five bytes from eight byte type.
    /// \param logLevel Custom logging level.
    static void startConsoleLogging(uint8_t logLevel);

    /// Shut down logging levels and remove handlers.
    static void stopLogging();

    /// Add handler for logging messages.
    template <typename T>
    static void addHandler(T&& handler)
    {
        log.addHandler(std::move(handler));
    }

    /// Remove handle that logs messages.
    template <typename T>
    static void removeHandler(T&& handler)
    {
        log.removeHandler(std::forward<T>(handler));
    }

    /// Remove all handlers.
    static void clearHandlers()
    {
        log.clearHandlers();
    }

    /// Check if there is existing handlers.
    static bool noHandlers()
    {
        return log.noHandlers();
    }

public:
    /// LogLevel handling functions.

    static bool setLevel(logger::LogLevel logLevel)
    {
        return log.setLevel(logLevel);
    }

    static bool unsetLevel(logger::LogLevel logLevel)
    {
        return log.unsetLevel(logLevel);
    }

    static bool getLevel(logger::LogLevel logLevel)
    {
        return log.getLevel(logLevel);
    }

    static bool setMinimumLevel(logger::LogLevel logLevel)
    {
        return log.setMinimumLevel(logLevel);
    }

public:
    /// All the overloaded printing functions

    template <typename T, typename... Args>
    static void debug(const T& data, const Args&... args)
    {
        log.debug(data, args...);
    }

    template <typename T, typename... Args>
    static void info(const T& data, const Args&... args)
    {
        log.info(data, args...);
    }

    template <typename T, typename... Args>
    static void warning(const T& data, const Args&... args)
    {
        log.warning(data, args...);
    }

    template <typename T, typename... Args>
    static void error(const T& data, const Args&... args)
    {
        log.error(data, args...);
    }

    template <typename T, typename... Args>
    static void critical(const T& data, const Args&... args)
    {
        log.critical(data, args...);
    }

private:
    static logger::Logger log;
};

} // namespace nexilis

#endif
