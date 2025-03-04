#ifndef NEXILISC_SERVER_AUTHENTICATION_MODE_C_H
#define NEXILISC_SERVER_AUTHENTICATION_MODE_C_H

#ifdef __cplusplus
extern "C" {
#endif

// Enum for AuthenticationMode
typedef enum {
    AUTHENTICATION_MODE_FREE,
    AUTHENTICATION_MODE_PASSWORD_PROTECTED,
    AUTHENTICATION_MODE_WHITELISTED
} nexilis_server_AuthenticationModeC;

#ifdef __cplusplus
}
#endif

#endif
