#include <nexilis/logger/log.hh>
#include <nexilis/logger/console_handler.hh>

namespace nexilis
{

logger::Logger Log::log;

void Log::startConsoleLogging(logger::LogLevel minLevel)
{
    log.setMinimumLevel(minLevel);
    log.addHandler(logger::ConsoleHandler());
}

void Log::startConsoleDebugging()
{
    log.setMinimumLevel(logger::LogLevel::DEBUG);
    log.addHandler(logger::ConsoleHandler());
}

void Log::stopLogging()
{
    log.unsetLevel(logger::LogLevel::CRITICAL);
    log.unsetLevel(logger::LogLevel::ERROR);
    log.unsetLevel(logger::LogLevel::WARNING);
    log.unsetLevel(logger::LogLevel::INFO);
    log.unsetLevel(logger::LogLevel::DEBUG);

    clearHandlers();
}

void Log::startConsoleLogging(uint8_t logLevel)
{
    log.setLogLevel(logLevel);

    log.addHandler(logger::ConsoleHandler());
}

} // namespace nexilis
