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
