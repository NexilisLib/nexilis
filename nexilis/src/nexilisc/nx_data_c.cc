#include <nexilisc/nx_data_c.h>

#include <cstdlib>
#include <cstring>

nx_data_c nx_data_create(size_t size)
{
    nx_data_c result;
    result.data = static_cast<uint8_t*>(malloc(size));
    result.size = size;
    return result;
}

nx_data_c nx_data_create_from(const uint8_t* data, size_t size)
{
    nx_data_c result = nx_data_create(size);
    if (result.data && data)
    {
        memcpy(result.data, data, size);
    }
    return result;
}

void nx_data_destroy(nx_data_c* data)
{
    if (data && data->data)
    {
        free(data->data);
        data->data = nullptr;
        data->size = 0;
    }
}
