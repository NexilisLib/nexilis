#include <nexilis/log.hh>
#include <nexilis/logger/console_handler.hh>

namespace nexilis
{

Logger Log::log;

void Log::startConsoleLogging(LogLevel minLevel)
{
    log.setMinimumLevel(minLevel);

    log.addHandler(ConsoleHandler());
}

void Log::stopLogging()
{
    log.unsetLevel(LogLevel::CRITICAL);
    log.unsetLevel(LogLevel::ERROR);
    log.unsetLevel(LogLevel::WARNING);
    log.unsetLevel(LogLevel::INFO);
    log.unsetLevel(LogLevel::DEBUG);

    clearHandlers();
}

void Log::startConsoleLogging(uint8_t logLevel)
{
    log.setLogLevel(logLevel);

    log.addHandler(ConsoleHandler());
}

} // namespace nexilis
