#include <nexilisc/logger/base_handler_c.h>

#include <nexilis/logger/base_handler.hh>

using namespace nexilis::logger;

extern "C"
{

    struct nexilis_logger_BaseHandler
    {
        BaseHandler* handler;
    };

    uint64_t nexilis_logger_BaseHandler_getId(nexilis_logger_BaseHandler* handler)
    {
        auto base_handler = reinterpret_cast<nexilis::logger::BaseHandler*>(handler);
        return base_handler->getId();
    }
}
