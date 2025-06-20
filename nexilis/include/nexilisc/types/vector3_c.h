#ifndef NEXILISC_TYPES_VECTOR3_C_H
#define NEXILISC_TYPES_VECTOR3_C_H

#include <nexilis/types/vector3.hh>

#ifdef __cplusplus
extern "C" {
#endif

struct nexilis_Vector3f
{
    nexilis::Vector3f* vec;
};

struct nexilis_Vector3u
{
    nexilis::Vector3u* vec;
};

struct nexilis_Vector3i
{
    nexilis::Vector3i* vec;
};

// nexilis_Vector3f API
nexilis_Vector3f* nexilis_vector3f_create(float x, float y, float z);
nexilis_Vector3f* nexilis_vector3f_create_default();
void nexilis_vector3f_destroy(nexilis_Vector3f* vec);
float nexilis_vector3f_get_x(const nexilis_Vector3f* vec);
float nexilis_vector3f_get_y(const nexilis_Vector3f* vec);
float nexilis_vector3f_get_z(const nexilis_Vector3f* vec);
void nexilis_vector3f_set_x(nexilis_Vector3f* vec, float x);
void nexilis_vector3f_set_y(nexilis_Vector3f* vec, float y);
void nexilis_vector3f_set_z(nexilis_Vector3f* vec, float z);
void nexilis_vector3f_serialize(const nexilis_Vector3f* vec, uint8_t* out_data);
nexilis_Vector3f* nexilis_vector3f_deserialize(const uint8_t* data);
bool nexilis_vector3f_is_valid(void* vector_ptr);

// nexilis_Vector3u API
/*
nexilis_Vector3u* nexilis_vector3u_create(uint64_t x, uint64_t y, uint64_t z);
nexilis_Vector3u* nexilis_vector3u_create_default();
void nexilis_vector3u_destroy(nexilis_Vector3u* vec);
uint64_t nexilis_vector3u_get_x(const nexilis_Vector3u* vec);
uint64_t nexilis_vector3u_get_y(const nexilis_Vector3u* vec);
uint64_t nexilis_vector3u_get_z(const nexilis_Vector3u* vec);
void nexilis_vector3u_set_x(nexilis_Vector3u* vec, uint64_t x);
void nexilis_vector3u_set_y(nexilis_Vector3u* vec, uint64_t y);
void nexilis_vector3u_set_z(nexilis_Vector3u* vec, uint64_t z);
void nexilis_vector3u_serialize(const nexilis_Vector3u* vec, uint8_t* out_data);
nexilis_Vector3u* nexilis_vector3u_deserialize(const uint8_t* data);

// nexilis_Vector3i API
nexilis_Vector3i* nexilis_Vector3i_create(int x, int y, int z);
nexilis_Vector3i* nexilis_Vector3i_create_default();
void nexilis_Vector3i_destroy(nexilis_Vector3i* vec);
int nexilis_Vector3i_get_x(const nexilis_Vector3i* vec);
int nexilis_Vector3i_get_y(const nexilis_Vector3i* vec);
int nexilis_Vector3i_get_z(const nexilis_Vector3i* vec);
void nexilis_Vector3i_set_x(nexilis_Vector3i* vec, int x);
void nexilis_Vector3i_set_y(nexilis_Vector3i* vec, int y);
void nexilis_Vector3i_set_z(nexilis_Vector3i* vec, int z);
void nexilis_Vector3i_serialize(const nexilis_Vector3i* vec, uint8_t* out_data);
nexilis_Vector3i* nexilis_Vector3i_deserialize(const uint8_t* data);
*/

#ifdef __cplusplus
}
#endif

#endif // VECTOR3_C_H
