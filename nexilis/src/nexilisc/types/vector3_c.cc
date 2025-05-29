#include <nexilisc/types/vector3_c.h>

#include <cassert>
#include <cstring>
#include <vector>

// nexilis_Vector3f implementation
nexilis_Vector3f* nexilis_vector3f_create(float x, float y, float z)
{
    auto* vector = new nexilis_Vector3f;
    vector->vec = new nexilis::Vector3f(x, y, z);
    return vector;
}

nexilis_Vector3f* nexilis_vector3f_create_default()
{
    return new nexilis_Vector3f;
}

void nexilis_vector3f_destroy(nexilis_Vector3f* vec)
{
    if (vec)
    {
        if (vec->vec)
        {
            delete vec->vec;
        }
        delete vec;
    }
}

float nexilis_vector3f_get_x(const nexilis_Vector3f* vec)
{
    return vec->vec->x;
}

float nexilis_vector3f_get_y(const nexilis_Vector3f* vec)
{
    return vec->vec->y;
}

float nexilis_vector3f_get_z(const nexilis_Vector3f* vec)
{
    return vec->vec->z;
}

void nexilis_vector3f_set_x(nexilis_Vector3f* vec, float x)
{
    vec->vec->x = x;
}

void nexilis_vector3f_set_y(nexilis_Vector3f* vec, float y)
{
    vec->vec->y = y;
}

void nexilis_vector3f_set_z(nexilis_Vector3f* vec, float z)
{
    vec->vec->z = z;
}

void nexilis_vector3f_serialize(const nexilis_Vector3f* vec, uint8_t* out_data)
{
    auto data = vec->vec->serialize();
    memcpy(out_data, data.data(), data.size());
}

nexilis_Vector3f* nexilis_vector3f_deserialize(const uint8_t* data)
{
    size_t size = sizeof(data) * 3;
    nexilis::nx_data converted_data(data, data + size);
    auto* deserialized = new nexilis::Vector3f(nexilis::Vector3<float>::deserialize(converted_data));
    auto* new_vector = new nexilis_Vector3f;
    new_vector->vec = deserialized;
    return new_vector;
}

/*

// nexilis_Vector3u implementation
nexilis_Vector3u* nexilis_vector3u_create(uint64_t x, uint64_t y, uint64_t z)
{
    return new nexilis_Vector3u(x, y, z);
}

nexilis_Vector3u* nexilis_vector3u_create_default() {
    return new nexilis_Vector3u();
}

void nexilis_vector3u_destroy(nexilis_Vector3u* vec) {
    delete vec;
}

uint64_t nexilis_vector3u_get_x(const nexilis_Vector3u* vec) {
    return vec->x;
}

uint64_t nexilis_vector3u_get_y(const nexilis_Vector3u* vec) {
    return vec->y;
}

uint64_t nexilis_vector3u_get_z(const nexilis_Vector3u* vec) {
    return vec->z;
}

void nexilis_vector3u_set_x(nexilis_Vector3u* vec, uint64_t x) {
    vec->x = x;
}

void nexilis_vector3u_set_y(nexilis_Vector3u* vec, uint64_t y) {
    vec->y = y;
}

void nexilis_vector3u_set_z(nexilis_Vector3u* vec, uint64_t z) {
    vec->z = z;
}

void nexilis_vector3u_serialize(const nexilis_Vector3u* vec, uint8_t* out_data) {
    auto data = vec->serialize();
    std::memcpy(out_data, data.data(), data.size());
}

nexilis_Vector3u* nexilis_vector3u_deserialize(const uint8_t* data) {
    nx_data serialized(data, data + sizeof(uint32_t) * 3);
    try {
        auto vec = new nexilis_Vector3u(Vector3u::deserialize(serialized));
        return vec;
    } catch (...) {
        return nullptr;
    }
}

// nexilis_Vector3i implementation
nexilis_Vector3i* nexilis_vector3i_create(int x, int y, int z) {
    return new nexilis_Vector3i(x, y, z);
}

nexilis_Vector3i* nexilis_vector3i_create_default() {
    return new nexilis_Vector3i();
}

void nexilis_vector3i_destroy(nexilis_Vector3i* vec) {
    delete vec;
}

int nexilis_vector3i_get_x(const nexilis_Vector3i* vec) {
    return vec->x;
}

int nexilis_vector3i_get_y(const nexilis_Vector3i* vec) {
    return vec->y;
}

int nexilis_vector3i_get_z(const nexilis_Vector3i* vec) {
    return vec->z;
}

void nexilis_vector3i_set_x(nexilis_Vector3i* vec, int x) {
    vec->x = x;
}

void nexilis_vector3i_set_y(nexilis_Vector3i* vec, int y) {
    vec->y = y;
}

void nexilis_vector3i_set_z(nexilis_Vector3i* vec, int z) {
    vec->z = z;
}

void nexilis_vector3i_serialize(const nexilis_Vector3i* vec, uint8_t* out_data) {
    auto data = vec->serialize();
    std::memcpy(out_data, data.data(), data.size());
}

nexilis_Vector3i* nexilis_vector3i_deserialize(const uint8_t* data) {
    nx_data serialized(data, data + sizeof(uint32_t) * 3);
    try {
        auto vec = new nexilis_Vector3i(Vector3i::deserialize(serialized));
        return vec;
    } catch (...) {
        return nullptr;
    }
}

*/
