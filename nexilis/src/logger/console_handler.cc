#include <nexilis/logger/console_handler.hh>
#include <nexilis/common/util.hh>

namespace nexilis::logger
{

void ConsoleHandler::emit(LogLevel logLevel, const std::string& data)
{
    Util::sendColorMessageToConsole(logLevel, data);
}

}
