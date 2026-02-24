#include <nexilisc/logger/function_handler_c.h>

nexilis_logger_FunctionHandler* nexilis_logger_FunctionHandler_create(void (*handler)(const nexilis::logger::LogLevel&, const char*))
{
    auto function_handler = new nexilis_logger_FunctionHandler();

    auto func_handler = [handler](const nexilis::logger::LogLevel& level, const std::string& message)
    {
        handler(level, message.c_str());
    };
    function_handler->handler = new nexilis::logger::FunctionHandler(func_handler);
    return function_handler;
}

void nexilis_logger_FunctionHandler_destroy(nexilis_logger_FunctionHandler* handler)
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

uint64_t nexilis_logger_FunctionHandler_get_id(nexilis_logger_FunctionHandler* handler)
{
    if (handler && handler->handler)
    {
        return handler->handler->getId();
    }
    return 0;
}
