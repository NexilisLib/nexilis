#include <nexilis/log.hh>

namespace nexilis
{

Logger Log::log;

void Log::startConsoleLogging(LogLevel minLevel)
{
    log.setMinimumLevel(minLevel);

    log.addHandler(ConsoleHandler());
}

void Log::startConsoleLogging(uint8_t logLevel)
{
    log.setLogLevel(logLevel);

    log.addHandler(ConsoleHandler());
}

} // namespace nexilis
