#ifndef NEXILISC_LOGGER_CONSOLE_HANDLER_C_H
#define NEXILISC_LOGGER_CONSOLE_HANDLER_C_H

#include <nexilis/logger/console_handler.hh>

#ifdef __cplusplus
extern "C" {
#endif

struct nexilis_logger_ConsoleHandler
{
    nexilis::logger::ConsoleHandler* handler;
};

nexilis_logger_ConsoleHandler* nexilis_logger_ConsoleHandler_create();
void nexilis_logger_ConsoleHandler_destroy(nexilis_logger_ConsoleHandler* handler);

#ifdef __cplusplus
}
#endif

#endif
