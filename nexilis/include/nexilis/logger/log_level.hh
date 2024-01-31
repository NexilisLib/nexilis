#ifndef NEXILIS_LOGGER_LOGLEVEL_HH
#define NEXILIS_LOGGER_LOGLEVEL_HH

namespace nexilis::logger
{

/// Different levels of logging.
enum class LogLevel
{
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    CRITICAL
};

} // namespace nexilis::logger

#endif
