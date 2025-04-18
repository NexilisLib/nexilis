#ifndef NEXILISC_NX_DATA_C_H
#define NEXILISC_NX_DATA_C_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint8_t* data;
    size_t size;
} nx_data_c;

nx_data_c nexilis_nx_data_create(size_t size);
nx_data_c nexilis_nx_data_create_from(const uint8_t* data, size_t size);
void nexilis_nx_data_destroy(nx_data_c* data);

#ifdef __cplusplus
}
#endif

#endif