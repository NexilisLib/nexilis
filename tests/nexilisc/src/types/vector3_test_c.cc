#include <gtest/gtest.h>

#include <nexilisc/nx_data_c.h>
#include <nexilisc/types/vector3_c.h>

#include <cstring>
#include <vector>

TEST(Vector3fTest_c, CreateDefault)
{
    nexilis_Vector3f* v = nexilis_vector3f_create(0.0f, 0.0f, 0.0f);
    ASSERT_NE(v, nullptr);
    EXPECT_FLOAT_EQ(nexilis_vector3f_get_x(v), 0.0f);
    EXPECT_FLOAT_EQ(nexilis_vector3f_get_y(v), 0.0f);
    EXPECT_FLOAT_EQ(nexilis_vector3f_get_z(v), 0.0f);
    nexilis_vector3f_destroy(v);
}

TEST(Vector3fTest_c, CreateWithValues)
{
    nexilis_Vector3f* v = nexilis_vector3f_create(1.0f, 2.0f, 3.0f);
    ASSERT_NE(v, nullptr);
    EXPECT_FLOAT_EQ(nexilis_vector3f_get_x(v), 1.0f);
    EXPECT_FLOAT_EQ(nexilis_vector3f_get_y(v), 2.0f);
    EXPECT_FLOAT_EQ(nexilis_vector3f_get_z(v), 3.0f);
    nexilis_vector3f_destroy(v);
}

TEST(Vector3fTest_c, SetX)
{
    nexilis_Vector3f* v = nexilis_vector3f_create(0.0f, 0.0f, 0.0f);
    nexilis_vector3f_set_x(v, 5.5f);
    EXPECT_FLOAT_EQ(nexilis_vector3f_get_x(v), 5.5f);
    nexilis_vector3f_destroy(v);
}

TEST(Vector3fTest_c, SetY)
{
    nexilis_Vector3f* v = nexilis_vector3f_create(0.0f, 0.0f, 0.0f);
    nexilis_vector3f_set_y(v, -3.0f);
    EXPECT_FLOAT_EQ(nexilis_vector3f_get_y(v), -3.0f);
    nexilis_vector3f_destroy(v);
}

TEST(Vector3fTest_c, SetZ)
{
    nexilis_Vector3f* v = nexilis_vector3f_create(0.0f, 0.0f, 0.0f);
    nexilis_vector3f_set_z(v, 100.0f);
    EXPECT_FLOAT_EQ(nexilis_vector3f_get_z(v), 100.0f);
    nexilis_vector3f_destroy(v);
}

TEST(Vector3fTest_c, SerializeDeserialize)
{
    nexilis_Vector3f* original = nexilis_vector3f_create(1.5f, -2.5f, 3.0f);
    ASSERT_NE(original, nullptr);

    const size_t expected_size = sizeof(float) * 3;
    std::vector<uint8_t> buffer(expected_size);
    nexilis_vector3f_serialize(original, buffer.data());

    nexilis_Vector3f* deserialized = nexilis_vector3f_deserialize(buffer.data());
    ASSERT_NE(deserialized, nullptr);
    EXPECT_FLOAT_EQ(nexilis_vector3f_get_x(deserialized), 1.5f);
    EXPECT_FLOAT_EQ(nexilis_vector3f_get_y(deserialized), -2.5f);
    EXPECT_FLOAT_EQ(nexilis_vector3f_get_z(deserialized), 3.0f);

    nexilis_vector3f_destroy(original);
    nexilis_vector3f_destroy(deserialized);
}

TEST(Vector3fTest_c, SerializeDeserializeZero)
{
    nexilis_Vector3f* original = nexilis_vector3f_create(0.0f, 0.0f, 0.0f);

    const size_t expected_size = sizeof(float) * 3;
    std::vector<uint8_t> buffer(expected_size);
    nexilis_vector3f_serialize(original, buffer.data());

    nexilis_Vector3f* deserialized = nexilis_vector3f_deserialize(buffer.data());
    ASSERT_NE(deserialized, nullptr);
    EXPECT_FLOAT_EQ(nexilis_vector3f_get_x(deserialized), 0.0f);
    EXPECT_FLOAT_EQ(nexilis_vector3f_get_y(deserialized), 0.0f);
    EXPECT_FLOAT_EQ(nexilis_vector3f_get_z(deserialized), 0.0f);

    nexilis_vector3f_destroy(original);
    nexilis_vector3f_destroy(deserialized);
}

TEST(Vector3fTest_c, DeserializeNullReturnsNull)
{
    nexilis_Vector3f* result = nexilis_vector3f_deserialize(nullptr);
    EXPECT_EQ(result, nullptr);
}

TEST(Vector3fTest_c, IsValidWithValidVector)
{
    nexilis_Vector3f* v = nexilis_vector3f_create(1.0f, 2.0f, 3.0f);
    EXPECT_TRUE(nexilis_vector3f_is_valid(v));
    nexilis_vector3f_destroy(v);
}

TEST(Vector3fTest_c, IsValidWithNull)
{
    EXPECT_FALSE(nexilis_vector3f_is_valid(nullptr));
}

TEST(Vector3fTest_c, IsValidWithNaN)
{
    nexilis_Vector3f* v = nexilis_vector3f_create(0.0f, 0.0f, 0.0f);
    nexilis_vector3f_set_x(v, std::numeric_limits<float>::quiet_NaN());
    EXPECT_FALSE(nexilis_vector3f_is_valid(v));
    nexilis_vector3f_destroy(v);
}

TEST(Vector3fTest_c, IsValidWithInfinity)
{
    nexilis_Vector3f* v = nexilis_vector3f_create(0.0f, 0.0f, 0.0f);
    nexilis_vector3f_set_y(v, std::numeric_limits<float>::infinity());
    EXPECT_FALSE(nexilis_vector3f_is_valid(v));
    nexilis_vector3f_destroy(v);
}
