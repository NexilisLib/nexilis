#include <nexilis/log.hh>

namespace nexilis
{

Logger Log::log;

void Log::startConsoleLogging(LogLevel minLevel)
{
    log.setMinimumLevel(minLevel);

    log.addHandler(ConsoleHandler());
}

}
