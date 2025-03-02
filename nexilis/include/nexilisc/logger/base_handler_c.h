#ifndef NEXILISC_LOGGER_BASE_HANDLER_C_H
#define NEXILISC_LOGGER_BASE_HANDLER_C_H

#include <nexilisc/logger/log_level_c.h>

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct nexilis_logger_BaseHandler nexilis_logger_BaseHandler;
uint64_t nexilis_logger_BaseHandler_getId(nexilis_logger_BaseHandler* handler);

#ifdef __cplusplus
}
#endif

#endif
