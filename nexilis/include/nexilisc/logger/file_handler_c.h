#ifndef NEXILISC_LOGGER_FILE_HANDLER_C_H
#define NEXILISC_LOGGER_FILE_HANDLER_C_H

#include <nexilis/logger/file_handler.hh>

#ifdef __cplusplus
extern "C" {
#endif

struct nexilis_logger_FileHandler
{
    nexilis::logger::FileHandler* handler;
};

nexilis_logger_FileHandler* nexilis_logger_FileHandler_create(const char* filename);
void nexilis_logger_FileHandler_destroy(nexilis_logger_FileHandler* handler);

#ifdef __cplusplus
}
#endif

#endif
