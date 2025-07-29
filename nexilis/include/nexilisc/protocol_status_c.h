#ifndef NEXILISC_PROTOCOL_STATUS_C_H
#define NEXILISC_PROTOCOL_STATUS_C_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    PROTOCOL_STATUS_UNDEFINED,
    PROTOCOL_STATUS_CONNECTING,
    PROTOCOL_STATUS_CONNECTED,
    PROTOCOL_STATUS_SWITCHING_PORTS,
    PROTOCOL_STATUS_ERROR
} nexilis_ProtocolStatusC;

#ifdef __cplusplus
}
#endif

#endif
