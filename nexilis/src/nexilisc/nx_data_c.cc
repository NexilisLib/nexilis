#include <nexilisc/nx_data_c.h>

#include <cstdlib>
#include <cstring>

nx_data_c nexilis_nx_data_create(uint64_t size)
{
    nx_data_c result;
    result.data = new nexilis::nx_data(size);
    return result;
}

nx_data_c nexilis_nx_data_create_from(const uint8_t* data, uint64_t size)
{
    nx_data_c result;
    auto* vec = new nexilis::nx_data(size);
    if (data && size > 0)
    {
        memcpy(vec->data(), data, size);
    }
    result.data = vec;
    return result;
}

void nexilis_nx_data_destroy(nx_data_c* data)
{
    if (data && data->data)
    {
        delete data->data;
        data->data = nullptr;
    }
}

uint64_t nexilis_nx_data_get_size(const nx_data_c* data)
{
    if (!data || !data->data)
    {
        return 0;
    }
    return data->data->size();
}

const uint8_t* nexilis_nx_data_get_data(const nx_data_c* data)
{
    if (!data || !data->data)
    {
        return nullptr;
    }
    auto* vec = static_cast<nexilis::nx_data*>(data->data);
    return vec->empty() ? nullptr : vec->data();
}

uint8_t* nexilis_nx_data_get_mutable(nx_data_c* data)
{
    if (!data || !data->data)
    {
        return nullptr;
    }
    auto* vec = static_cast<nexilis::nx_data*>(data->data);
    return vec->empty() ? nullptr : vec->data();
}
