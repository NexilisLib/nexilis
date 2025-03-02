#ifndef NEXILISC_LOGGER_CONSOLE_HANDLER_C_H
#define NEXILISC_LOGGER_CONSOLE_HANDLER_C_H

#ifdef __cplusplus
extern "C" {
#endif

#include <nexilisc/logger/log_level_c.h>

typedef struct nexilis_logger_ConsoleHandler nexilis_logger_ConsoleHandler;

nexilis_logger_ConsoleHandler* nexilis_logger_ConsoleHandler_create();
void nexilis_logger_ConsoleHandler_destroy(nexilis_logger_ConsoleHandler* handler);
void nexilis_logger_ConsoleHandler_emit(nexilis_logger_ConsoleHandler* handler, nexilis_logger_loglevel log_level, const char* data);

#ifdef __cplusplus
}
#endif

#endif
