#include <nexilis/logger/logger.hh>
#include <nexilis/logger/log_level.hh>
#include <nexilis/logger/console_handler.hh>

namespace nexilis
{

class Log
{
public:
    static void startConsoleLogging(LogLevel minLevel = LogLevel::INFO);

    template <typename T>
    static void addHandler(T&& handler)
    {
        log.addHandler(std::move(handler));
    }

    template <typename T, typename ...Args>
    static void debug(const T& data, const Args&... args)
    {
        log.debug(data, args...);
    }

    template <typename T, typename ...Args>
    static void info(const T& data, const Args&... args)
    {
        log.info(data, args...);
    }

    template <typename T, typename ...Args>
    static void warning(const T& data, const Args&... args)
    {
        log.warning(data, args...);
    }

    template <typename T, typename ...Args>
    static void error(const T& data, const Args&... args)
    {
        log.error(data, args...);
    }

    template <typename T, typename ...Args>
    static void critical(const T& data, const Args&... args)
    {
        log.critical(data, args...);
    }

private:
    static Logger log;
};

}
