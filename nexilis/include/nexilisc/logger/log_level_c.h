#ifndef NEXILISC_LOGGER_LOG_LEVEL_C_H
#define NEXILISC_LOGGER_LOG_LEVEL_C_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    NEXILIS_LOGGER_LOGLEVEL_DEBUG,
    NEXILIS_LOGGER_LOGLEVEL_INFO,
    NEXILIS_LOGGER_LOGLEVEL_WARNING,
    NEXILIS_LOGGER_LOGLEVEL_ERROR,
    NEXILIS_LOGGER_LOGLEVEL_CRITICAL
} nexilis_logger_loglevel;

#ifdef __cplusplus
}
#endif

#endif
