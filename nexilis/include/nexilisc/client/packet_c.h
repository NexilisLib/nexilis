#ifndef NEXILISC_CLIENT_PACKET_C_H
#define NEXILISC_CLIENT_PACKET_C_H

#include <nexilisc/nx_data_c.h>

#ifdef __cplusplus
extern "C" {
#endif

nx_data_c nexilis_packet_info_general();
nx_data_c nexilis_packet_info_clients();
nx_data_c nexilis_packet_info_rooms();

#ifdef __cplusplus
}
#endif

#endif