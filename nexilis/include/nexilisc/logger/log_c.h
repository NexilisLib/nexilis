#ifndef NEXILISC_LOGGER_LOG_C_H
#define NEXILISC_LOGGER_LOG_C_H

#include <nexilisc/logger/log_level_c.h>
#include <nexilis/logger/log_level.hh>

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/// Start console logging session with a minimum log level.
void nexilis_log_start_console_logging(nexilis_logger_loglevel minLevel);

/// Start console logging session with all log levels enabled.
void nexilis_log_start_console_debugging();

/// Start console logging with a custom log level.
void nexilis_log_start_console_logging_custom(uint8_t logLevel);

/// Stop logging and remove all handlers.
void nexilis_log_stop_logging();

/// Add a console handler (returns a handler ID).
uint64_t nexilis_log_add_console_handler();

/// Add a file handler (returns a handler ID).
uint64_t nexilis_log_add_file_handler(const char* filename);

/// Add a function handler (returns a handler ID).
uint64_t nexilis_log_add_function_handler(void (*handler)(const nexilis_logger_loglevel&, const char*));

/// Remove a log handler by ID.
void nexilis_log_remove_handler(uint64_t handlerId);

/// Remove all log handlers.
void nexilis_log_clear_handlers();

/// Check if there are no active log handlers.
bool nexilis_log_no_handlers();

/// Set a specific logging level.
bool nexilis_log_set_level(nexilis_logger_loglevel logLevel);

/// Unset (disable) a specific logging level.
bool nexilis_log_unset_level(nexilis_logger_loglevel logLevel);

/// Check if a logging level is enabled.
bool nexilis_log_get_level(nexilis_logger_loglevel logLevel);

/// Set the minimum logging level.
bool nexilis_log_set_minimum_level(nexilis_logger_loglevel logLevel);

/// Log messages at different levels.
void nexilis_log_debug(const char* message);
void nexilis_log_info(const char* message);
void nexilis_log_warning(const char* message);
void nexilis_log_error(const char* message);
void nexilis_log_critical(const char* message);

#ifdef __cplusplus
}
#endif

#endif
