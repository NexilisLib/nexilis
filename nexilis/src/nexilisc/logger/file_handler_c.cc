#include <nexilisc/logger/file_handler_c.h>

nexilis_logger_FileHandler* nexilis_logger_FileHandler_create(const char* filename)
{
    auto file_handler = new nexilis_logger_FileHandler();
    file_handler->handler = new nexilis::logger::FileHandler(filename);
    return file_handler;
}

void nexilis_logger_FileHandler_destroy(nexilis_logger_FileHandler* handler)
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

uint64_t nexilis_logger_FileHandler_get_id(nexilis_logger_FileHandler* handler)
{
    if (handler && handler->handler)
    {
        return handler->handler->getId();
    }
    return 0;
}
