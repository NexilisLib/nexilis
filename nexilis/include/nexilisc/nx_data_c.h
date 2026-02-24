#ifndef NEXILISC_NX_DATA_C_H
#define NEXILISC_NX_DATA_C_H

#include <nexilis/nx_data.hh>

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

    struct nx_data_c
    {
        nexilis::nx_data* data;
    };

    nx_data_c nexilis_nx_data_create(uint64_t size);
    nx_data_c nexilis_nx_data_create_from(const uint8_t* data, uint64_t size);
    void nexilis_nx_data_destroy(nx_data_c* data);
    uint64_t nexilis_nx_data_get_size(const nx_data_c* data);
    const uint8_t* nexilis_nx_data_get_data(const nx_data_c* data);
    uint8_t* nexilis_nx_data_get_mutable(nx_data_c* data);

#ifdef __cplusplus
}
#endif

#endif
