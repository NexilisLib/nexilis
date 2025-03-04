#include <nexilisc/logger/console_handler_c.h>

nexilis_logger_ConsoleHandler* nexilis_logger_ConsoleHandler_create()
{
    auto console_handler = new nexilis_logger_ConsoleHandler();
    console_handler->handler = new nexilis::logger::ConsoleHandler();
    return console_handler;
}

void nexilis_logger_ConsoleHandler_destroy(nexilis_logger_ConsoleHandler* handler)
{
    if (handler)
    {
        if (handler->handler)
        {
            delete handler->handler;
            handler->handler = nullptr;
        }
        delete handler;
    }
}
