#include <nexilisc/logger/console_handler_c.h>

#include <nexilis/logger/console_handler.hh>

extern "C"
{

    struct nexilis_logger_ConsoleHandler
    {
        nexilis::logger::ConsoleHandler* handler;
    };

    nexilis_logger_ConsoleHandler* nexilis_logger_ConsoleHandler_create()
    {
        return reinterpret_cast<nexilis_logger_ConsoleHandler*>(new nexilis::logger::ConsoleHandler());
    }

    void nexilis_logger_ConsoleHandler_destroy(nexilis_logger_ConsoleHandler* handler)
    {
        delete reinterpret_cast<nexilis::logger::ConsoleHandler*>(handler);
    }

    void nexilis_logger_ConsoleHandler_emit(nexilis_logger_ConsoleHandler* handler, nexilis_logger_loglevel log_level, const char* data)
    {
        auto file_handler = reinterpret_cast<nexilis::logger::ConsoleHandler*>(handler);
        file_handler->emit(static_cast<nexilis::logger::LogLevel>(log_level), data);
    }
}
