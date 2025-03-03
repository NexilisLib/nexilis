#ifndef NEXILISC_LOGGER_C_H
#define NEXILISC_LOGGER_C_H

#include <nexilisc/logger/log_level_c.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

typedef struct nexilis_logger_FileHandler nexilis_logger_FileHandler;
typedef struct nexilis_logger_ConsoleHandler nexilis_logger_ConsoleHandler;
typedef struct nexilis_logger_LoggerC nexilis_logger_LoggerC;

nexilis_logger_FileHandler* nexilis_logger_FileHandler_create(const char* filename);
void nexilis_logger_FileHandler_destroy(nexilis_logger_FileHandler* handler);

nexilis_logger_ConsoleHandler* nexilis_logger_ConsoleHandler_create();
void nexilis_logger_ConsoleHandler_destroy(nexilis_logger_ConsoleHandler* handler);

nexilis_logger_LoggerC* nexilis_logger_create();
void nexilis_logger_destroy(nexilis_logger_LoggerC* logger);

void nexilis_logger_add_file_handler(nexilis_logger_LoggerC* logger, nexilis_logger_FileHandler* handler);
void nexilis_logger_add_console_handler(nexilis_logger_LoggerC* logger, nexilis_logger_ConsoleHandler* handler);

void nexilis_logger_remove_handler(nexilis_logger_LoggerC* logger, uint64_t handlerId);
void nexilis_logger_clear_handlers(nexilis_logger_LoggerC* logger);
int nexilis_logger_no_handlers(nexilis_logger_LoggerC* logger);

void nexilis_logger_debug(nexilis_logger_LoggerC* logger, const char* message);
void nexilis_logger_info(nexilis_logger_LoggerC* logger, const char* message);
void nexilis_logger_warning(nexilis_logger_LoggerC* logger, const char* message);
void nexilis_logger_error(nexilis_logger_LoggerC* logger, const char* message);
void nexilis_logger_critical(nexilis_logger_LoggerC* logger, const char* message);

bool nexilis_logger_unset_level(nexilis_logger_LoggerC* logger, int level);
bool nexilis_logger_set_level(nexilis_logger_LoggerC* logger, int level);
bool nexilis_logger_get_level(nexilis_logger_LoggerC* logger, int level);
bool nexilis_logger_set_minimum_level(nexilis_logger_LoggerC* logger, int level);
void nexilis_logger_set_log_level(nexilis_logger_LoggerC* logger, uint8_t level);

#ifdef __cplusplus
}
#endif

#endif // NEXILISC_LOGGER_C_H

