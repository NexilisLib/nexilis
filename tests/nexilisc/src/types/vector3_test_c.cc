/* Copyright (C) 2026 Valtteri Viirret
   This file is part of the Nexilis Project.

   This file is free software: you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as
   published by the Free Software Foundation, either version 3 of the
   License, or (at your option) any later version.

   This file is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with this file.  If not, see <https://gnu.org>. */

#include <gtest/gtest.h>

#include <nexilisc/nx_data_c.h>
#include <nexilisc/types/vector3_c.h>

#include <cstdint>
#include <cstring>
#include <limits>
#include <type_traits>
#include <vector>

namespace
{

template <typename T, typename U>
void expect_components_equal(T actual, U expected)
{
    if constexpr (std::is_floating_point_v<T>)
    {
        EXPECT_FLOAT_EQ(actual, expected);
    }
    else
    {
        EXPECT_EQ(actual, expected);
    }
}

struct Vector3fAdapter
{
    using component_type = float;
    using vector_type = nexilis_Vector3f;

    static constexpr component_type x = 1.5f;
    static constexpr component_type y = -2.5f;
    static constexpr component_type z = 3.0f;
    static constexpr component_type new_x = 5.5f;
    static constexpr component_type new_y = -3.0f;
    static constexpr component_type new_z = 100.0f;

    static vector_type* create(component_type x, component_type y, component_type z)
    {
        return nexilis_vector3f_create(x, y, z);
    }
    static vector_type* create_default()
    {
        return nexilis_vector3f_create_default();
    }
    static void destroy(vector_type* vec)
    {
        nexilis_vector3f_destroy(vec);
    }
    static component_type get_x(const vector_type* vec)
    {
        return nexilis_vector3f_get_x(vec);
    }
    static component_type get_y(const vector_type* vec)
    {
        return nexilis_vector3f_get_y(vec);
    }
    static component_type get_z(const vector_type* vec)
    {
        return nexilis_vector3f_get_z(vec);
    }
    static void set_x(vector_type* vec, component_type x)
    {
        nexilis_vector3f_set_x(vec, x);
    }
    static void set_y(vector_type* vec, component_type y)
    {
        nexilis_vector3f_set_y(vec, y);
    }
    static void set_z(vector_type* vec, component_type z)
    {
        nexilis_vector3f_set_z(vec, z);
    }
    static void serialize(const vector_type* vec, uint8_t* out_data)
    {
        nexilis_vector3f_serialize(vec, out_data);
    }
    static vector_type* deserialize(const uint8_t* data)
    {
        return nexilis_vector3f_deserialize(data);
    }
    static bool is_valid(void* vector_ptr)
    {
        return nexilis_vector3f_is_valid(vector_ptr);
    }
};

struct Vector3iAdapter
{
    using component_type = int;
    using vector_type = nexilis_Vector3i;

    static constexpr component_type x = 1;
    static constexpr component_type y = -2;
    static constexpr component_type z = 3;
    static constexpr component_type new_x = 42;
    static constexpr component_type new_y = -7;
    static constexpr component_type new_z = 1000;

    static vector_type* create(component_type x, component_type y, component_type z)
    {
        return nexilis_vector3i_create(x, y, z);
    }
    static vector_type* create_default()
    {
        return nexilis_vector3i_create_default();
    }
    static void destroy(vector_type* vec)
    {
        nexilis_vector3i_destroy(vec);
    }
    static component_type get_x(const vector_type* vec)
    {
        return nexilis_vector3i_get_x(vec);
    }
    static component_type get_y(const vector_type* vec)
    {
        return nexilis_vector3i_get_y(vec);
    }
    static component_type get_z(const vector_type* vec)
    {
        return nexilis_vector3i_get_z(vec);
    }
    static void set_x(vector_type* vec, component_type x)
    {
        nexilis_vector3i_set_x(vec, x);
    }
    static void set_y(vector_type* vec, component_type y)
    {
        nexilis_vector3i_set_y(vec, y);
    }
    static void set_z(vector_type* vec, component_type z)
    {
        nexilis_vector3i_set_z(vec, z);
    }
    static void serialize(const vector_type* vec, uint8_t* out_data)
    {
        nexilis_vector3i_serialize(vec, out_data);
    }
    static vector_type* deserialize(const uint8_t* data)
    {
        return nexilis_vector3i_deserialize(data);
    }
    static bool is_valid(void* vector_ptr)
    {
        return nexilis_vector3i_is_valid(vector_ptr);
    }
};

struct Vector3uAdapter
{
    using component_type = uint64_t;
    using vector_type = nexilis_Vector3u;

    static constexpr component_type x = 1;
    static constexpr component_type y = 2;
    static constexpr component_type z = 3;
    static constexpr component_type new_x = 42;
    static constexpr component_type new_y = 7;
    static constexpr component_type new_z = 1000;

    static vector_type* create(component_type x, component_type y, component_type z)
    {
        return nexilis_vector3u_create(x, y, z);
    }
    static vector_type* create_default()
    {
        return nexilis_vector3u_create_default();
    }
    static void destroy(vector_type* vec)
    {
        nexilis_vector3u_destroy(vec);
    }
    static component_type get_x(const vector_type* vec)
    {
        return nexilis_vector3u_get_x(vec);
    }
    static component_type get_y(const vector_type* vec)
    {
        return nexilis_vector3u_get_y(vec);
    }
    static component_type get_z(const vector_type* vec)
    {
        return nexilis_vector3u_get_z(vec);
    }
    static void set_x(vector_type* vec, component_type x)
    {
        nexilis_vector3u_set_x(vec, x);
    }
    static void set_y(vector_type* vec, component_type y)
    {
        nexilis_vector3u_set_y(vec, y);
    }
    static void set_z(vector_type* vec, component_type z)
    {
        nexilis_vector3u_set_z(vec, z);
    }
    static void serialize(const vector_type* vec, uint8_t* out_data)
    {
        nexilis_vector3u_serialize(vec, out_data);
    }
    static vector_type* deserialize(const uint8_t* data)
    {
        return nexilis_vector3u_deserialize(data);
    }
    static bool is_valid(void* vector_ptr)
    {
        return nexilis_vector3u_is_valid(vector_ptr);
    }
};

} // namespace

template <typename T>
class Vector3Test_c : public testing::Test
{
};

using Vector3CVectorTypes = testing::Types<Vector3fAdapter, Vector3iAdapter, Vector3uAdapter>;
TYPED_TEST_SUITE(Vector3Test_c, Vector3CVectorTypes);

TYPED_TEST(Vector3Test_c, CreateDefault)
{
    auto* vec = TypeParam::create(0, 0, 0);
    ASSERT_NE(vec, nullptr);
    expect_components_equal(TypeParam::get_x(vec), 0);
    expect_components_equal(TypeParam::get_y(vec), 0);
    expect_components_equal(TypeParam::get_z(vec), 0);
    TypeParam::destroy(vec);
}

TYPED_TEST(Vector3Test_c, CreateDefaultFactory)
{
    using component_type = typename TypeParam::component_type;

    auto* vec = TypeParam::create_default();
    ASSERT_NE(vec, nullptr);
    expect_components_equal(TypeParam::get_x(vec), component_type{0});
    expect_components_equal(TypeParam::get_y(vec), component_type{0});
    expect_components_equal(TypeParam::get_z(vec), component_type{0});
    TypeParam::destroy(vec);
}

TYPED_TEST(Vector3Test_c, CreateWithValues)
{
    auto* vec = TypeParam::create(TypeParam::x, TypeParam::y, TypeParam::z);
    ASSERT_NE(vec, nullptr);
    expect_components_equal(TypeParam::get_x(vec), TypeParam::x);
    expect_components_equal(TypeParam::get_y(vec), TypeParam::y);
    expect_components_equal(TypeParam::get_z(vec), TypeParam::z);
    TypeParam::destroy(vec);
}

TYPED_TEST(Vector3Test_c, SetX)
{
    auto* vec = TypeParam::create(0, 0, 0);
    TypeParam::set_x(vec, TypeParam::new_x);
    expect_components_equal(TypeParam::get_x(vec), TypeParam::new_x);
    expect_components_equal(TypeParam::get_y(vec), 0);
    expect_components_equal(TypeParam::get_z(vec), 0);
    TypeParam::destroy(vec);
}

TYPED_TEST(Vector3Test_c, SetY)
{
    auto* vec = TypeParam::create(0, 0, 0);
    TypeParam::set_y(vec, TypeParam::new_y);
    expect_components_equal(TypeParam::get_x(vec), 0);
    expect_components_equal(TypeParam::get_y(vec), TypeParam::new_y);
    expect_components_equal(TypeParam::get_z(vec), 0);
    TypeParam::destroy(vec);
}

TYPED_TEST(Vector3Test_c, SetZ)
{
    auto* vec = TypeParam::create(0, 0, 0);
    TypeParam::set_z(vec, TypeParam::new_z);
    expect_components_equal(TypeParam::get_x(vec), 0);
    expect_components_equal(TypeParam::get_y(vec), 0);
    expect_components_equal(TypeParam::get_z(vec), TypeParam::new_z);
    TypeParam::destroy(vec);
}

TYPED_TEST(Vector3Test_c, SerializeDeserialize)
{
    auto* original = TypeParam::create(TypeParam::x, TypeParam::y, TypeParam::z);
    ASSERT_NE(original, nullptr);

    const size_t expected_size = sizeof(uint32_t) * 3;
    std::vector<uint8_t> buffer(expected_size);
    TypeParam::serialize(original, buffer.data());

    auto* deserialized = TypeParam::deserialize(buffer.data());
    ASSERT_NE(deserialized, nullptr);
    expect_components_equal(TypeParam::get_x(deserialized), TypeParam::x);
    expect_components_equal(TypeParam::get_y(deserialized), TypeParam::y);
    expect_components_equal(TypeParam::get_z(deserialized), TypeParam::z);

    TypeParam::destroy(original);
    TypeParam::destroy(deserialized);
}

TYPED_TEST(Vector3Test_c, SerializeDeserializeZero)
{
    auto* original = TypeParam::create(0, 0, 0);
    ASSERT_NE(original, nullptr);

    const size_t expected_size = sizeof(uint32_t) * 3;
    std::vector<uint8_t> buffer(expected_size);
    TypeParam::serialize(original, buffer.data());

    auto* deserialized = TypeParam::deserialize(buffer.data());
    ASSERT_NE(deserialized, nullptr);
    expect_components_equal(TypeParam::get_x(deserialized), 0);
    expect_components_equal(TypeParam::get_y(deserialized), 0);
    expect_components_equal(TypeParam::get_z(deserialized), 0);

    TypeParam::destroy(original);
    TypeParam::destroy(deserialized);
}

TYPED_TEST(Vector3Test_c, DeserializeNullReturnsNull)
{
    EXPECT_EQ(TypeParam::deserialize(nullptr), nullptr);
}

TYPED_TEST(Vector3Test_c, IsValidWithValidVector)
{
    auto* vec = TypeParam::create(TypeParam::x, TypeParam::y, TypeParam::z);
    EXPECT_TRUE(TypeParam::is_valid(vec));
    TypeParam::destroy(vec);
}

TYPED_TEST(Vector3Test_c, IsValidWithNull)
{
    EXPECT_FALSE(TypeParam::is_valid(nullptr));
}

TYPED_TEST(Vector3Test_c, IsValidWithNaN)
{
    using component_type = typename TypeParam::component_type;

    if constexpr (std::is_floating_point_v<component_type>)
    {
        auto* vec = TypeParam::create(0, 0, 0);
        TypeParam::set_x(vec, std::numeric_limits<component_type>::quiet_NaN());
        EXPECT_FALSE(TypeParam::is_valid(vec));
        TypeParam::destroy(vec);
    }
    else
    {
        GTEST_SKIP() << "NaN validity check only applies to floating-point vectors";
    }
}

TYPED_TEST(Vector3Test_c, IsValidWithInfinity)
{
    using component_type = typename TypeParam::component_type;

    if constexpr (std::is_floating_point_v<component_type>)
    {
        auto* vec = TypeParam::create(0, 0, 0);
        TypeParam::set_y(vec, std::numeric_limits<component_type>::infinity());
        EXPECT_FALSE(TypeParam::is_valid(vec));
        TypeParam::destroy(vec);
    }
    else
    {
        GTEST_SKIP() << "Infinity validity check only applies to floating-point vectors";
    }
}
