#include <nexilis/logger/console_handler.hh>
#include <nexilis/logger/log.hh>

namespace nexilis
{

logger::Logger Log::log;

void Log::startConsoleLogging(logger::LogLevel minLevel)
{
    log.setMinimumLevel(minLevel);
    log.addHandler(std::make_unique<logger::ConsoleHandler>(logger::ConsoleHandler()));
}

void Log::startConsoleDebugging()
{
    log.setMinimumLevel(logger::LogLevel::Debug);
    log.addHandler(std::make_unique<logger::ConsoleHandler>(logger::ConsoleHandler()));
}

void Log::stopLogging()
{
    log.unsetAllLevels();
    clearHandlers();
}

void Log::startConsoleLogging(uint8_t logLevel)
{
    log.setLogLevel(logLevel);

    log.addHandler(std::make_unique<logger::ConsoleHandler>(logger::ConsoleHandler()));
}

} // namespace nexilis
