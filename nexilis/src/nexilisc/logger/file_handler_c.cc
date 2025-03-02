#include <nexilisc/logger/file_handler_c.h>

#include <nexilis/logger/file_handler.hh>

extern "C"
{

    struct nexilis_logger_FileHandler
    {
        nexilis::logger::FileHandler* handler;
    };

    nexilis_logger_FileHandler* nexilis_logger_FileHandler_create(const char* filename)
    {
        return reinterpret_cast<nexilis_logger_FileHandler*>(new nexilis::logger::FileHandler(filename));
    }

    void nexilis_logger_FileHandler_destroy(nexilis_logger_FileHandler* handler)
    {
        delete reinterpret_cast<nexilis::logger::FileHandler*>(handler);
    }

    void nexilis_logger_FileHandler_emit(nexilis_logger_FileHandler* handler, nexilis_logger_loglevel log_level, const char* data)
    {
        auto file_handler = reinterpret_cast<nexilis::logger::FileHandler*>(handler);
        file_handler->emit(static_cast<nexilis::logger::LogLevel>(log_level), data);
    }
}
