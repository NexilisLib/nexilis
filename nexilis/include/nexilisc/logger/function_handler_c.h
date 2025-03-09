#ifndef NEXILIS_FUNCTION_HANDLER_C_H
#define NEXILIS_FUNCTION_HANDLER_C_H

#include <nexilis/logger/function_handler.hh>

#ifdef __cplusplus
extern "C" {
#endif

struct nexilis_logger_FunctionHandler
{
    nexilis::logger::FunctionHandler* handler;
};

nexilis_logger_FunctionHandler* nexilis_logger_FunctionHandler_create(void (*handler)(const nexilis::logger::LogLevel&, const char*));
void nexilis_logger_FunctionHandler_destroy(nexilis_logger_FunctionHandler* handler);
uint64_t nexilis_logger_FunctionHandler_get_id(nexilis_logger_FunctionHandler* handler);

#ifdef __cplusplus
}
#endif

#endif