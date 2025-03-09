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

uint64_t nexilis_logger_ConsoleHandler_get_id(nexilis_logger_ConsoleHandler* handler)
{
    if (handler && handler->handler)
    {
        return handler->handler->getId();
    }
    return 0;
}
