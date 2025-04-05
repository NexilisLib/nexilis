#ifndef NEXILIS_LOGGER_LOGLEVEL_HH
#define NEXILIS_LOGGER_LOGLEVEL_HH

#undef Debug
#undef Info
#undef Error
#undef Warning
#undef Critical

namespace nexilis::logger
{

/// Different levels of logging.
enum class LogLevel
{
    Debug,
    Info,
    Warning,
    Error,
    Critical
};

} // namespace nexilis::logger

#endif
