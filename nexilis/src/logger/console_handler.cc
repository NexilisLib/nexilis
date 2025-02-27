#include <nexilis/logger/console_handler.hh>
#include <nexilis/util.hh>

namespace nexilis::logger
{

void ConsoleHandler::emit(LogLevel logLevel, const std::string& data)
{
    Util::printColorMessageToConsole(logLevel, data);
}

} // namespace nexilis::logger
