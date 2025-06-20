#ifndef NEXILISC_SERVER_AUTHENTICATION_MODE_C_H
#define NEXILISC_SERVER_AUTHENTICATION_MODE_C_H

#ifdef __cplusplus
extern "C" {
#endif

// Enum for AuthenticationMode
typedef enum {
    AUTHENTICATION_MODE_EMPTY,
    AUTHENTICATION_MODE_SKIP,
    AUTHENTICATION_MODE_PASSWORD_PROTECTED,
    AUTHENTICATION_MODE_ADMIN_ACCESS,
    AUTHENTICATION_MODE_ROOT_ACCESS
} nexilis_server_AuthenticationModeC;

#ifdef __cplusplus
}
#endif

#endif
