#include <nexilisc/logger/log_c.h>

#include <nexilis/logger/console_handler.hh>
#include <nexilis/logger/file_handler.hh>
#include <nexilis/logger/function_handler.hh>
#include <nexilis/logger/log.hh>

void nexilis_log_start_console_logging(nexilis_logger_loglevel minLevel)
{
    nexilis::Log::startConsoleLogging(static_cast<nexilis::logger::LogLevel>(minLevel));
}

void nexilis_log_start_console_debugging()
{
    nexilis::Log::startConsoleDebugging();
}

void nexilis_log_start_console_logging_custom(uint8_t logLevel)
{
    nexilis::Log::startConsoleLogging(logLevel);
}

void nexilis_log_stop_logging()
{
    nexilis::Log::stopLogging();
}

uint64_t nexilis_log_add_console_handler()
{
    auto console_handler = std::make_unique<nexilis::logger::ConsoleHandler>();
    uint64_t handler_id = console_handler->getId();

    nexilis::Log::addHandler(std::move(console_handler));
    return handler_id;
}

uint64_t nexilis_log_add_file_handler(const char* filename)
{
    auto file_handler = std::make_unique<nexilis::logger::FileHandler>(filename);
    uint64_t handler_id = file_handler->getId();

    nexilis::Log::addHandler(std::move(file_handler));
    return handler_id;
}

uint64_t nexilis_log_add_function_handler(void (*handler)(const nexilis_logger_loglevel&, const char*))
{
    auto cpp_handler = [handler](const nexilis::logger::LogLevel& level, const std::string& message)
    {
        handler(static_cast<nexilis_logger_loglevel>(level), message.c_str());
    };

    auto function_handler = std::make_unique<nexilis::logger::FunctionHandler>(cpp_handler);
    uint64_t handler_id = function_handler->getId();

    nexilis::Log::addHandler(std::move(function_handler));
    return handler_id;
}

void nexilis_log_remove_handler(uint64_t handlerId)
{
    nexilis::Log::removeHandler(handlerId);
}

void nexilis_log_clear_handlers()
{
    nexilis::Log::clearHandlers();
}

bool nexilis_log_no_handlers()
{
    return nexilis::Log::noHandlers();
}

bool nexilis_log_set_level(nexilis_logger_loglevel logLevel)
{
    return nexilis::Log::setLevel(static_cast<nexilis::logger::LogLevel>(logLevel));
}

bool nexilis_log_unset_level(nexilis_logger_loglevel logLevel)
{
    return nexilis::Log::unsetLevel(static_cast<nexilis::logger::LogLevel>(logLevel));
}

bool nexilis_log_get_level(nexilis_logger_loglevel logLevel)
{
    return nexilis::Log::getLevel(static_cast<nexilis::logger::LogLevel>(logLevel));
}

bool nexilis_log_set_minimum_level(nexilis_logger_loglevel logLevel)
{
    return nexilis::Log::setMinimumLevel(static_cast<nexilis::logger::LogLevel>(logLevel));
}

void nexilis_log_debug(const char* message)
{
    nexilis::Log::debug(message);
}

void nexilis_log_info(const char* message)
{
    nexilis::Log::info(message);
}

void nexilis_log_warning(const char* message)
{
    nexilis::Log::warning(message);
}

void nexilis_log_error(const char* message)
{
    nexilis::Log::error(message);
}

void nexilis_log_critical(const char* message)
{
    nexilis::Log::critical(message);
}
