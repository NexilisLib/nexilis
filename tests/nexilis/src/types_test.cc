#include <gtest/gtest.h>

#include <nexilis/types/vector2.hh>
#include <nexilis/types/vector3.hh>

using namespace nexilis;

// ==================== Vector2 Tests ====================

TEST(Vector2Test, DefaultConstructor)
{
    Vector2f v;
    EXPECT_FLOAT_EQ(v.x, 0.0f);
    EXPECT_FLOAT_EQ(v.y, 0.0f);
}

TEST(Vector2Test, ParameterizedConstructor)
{
    Vector2f v(1.5f, 2.5f);
    EXPECT_FLOAT_EQ(v.x, 1.5f);
    EXPECT_FLOAT_EQ(v.y, 2.5f);
}

TEST(Vector2Test, CopyFromDifferentType)
{
    Vector2i vi(3, 4);
    Vector2f vf(vi);
    EXPECT_FLOAT_EQ(vf.x, 3.0f);
    EXPECT_FLOAT_EQ(vf.y, 4.0f);
}

TEST(Vector2Test, GetType)
{
    Vector2f v;
    EXPECT_EQ(v.getType(), VectorType::vector2);
}

TEST(Vector2Test, Equality)
{
    Vector2f a(1.0f, 2.0f);
    Vector2f b(1.0f, 2.0f);
    Vector2f c(3.0f, 4.0f);

    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a == c);
}

TEST(Vector2Test, Inequality)
{
    Vector2f a(1.0f, 2.0f);
    Vector2f b(1.0f, 2.0f);
    Vector2f c(3.0f, 4.0f);

    EXPECT_FALSE(a != b);
    EXPECT_TRUE(a != c);
}

TEST(Vector2Test, LessThan)
{
    Vector2f a(1.0f, 2.0f);
    Vector2f b(3.0f, 4.0f);
    Vector2f c(1.0f, 5.0f);

    EXPECT_TRUE(a < b);
    EXPECT_FALSE(b < a);
    EXPECT_TRUE(a < c);
    EXPECT_FALSE(c < a);
}

TEST(Vector2Test, GreaterThan)
{
    Vector2f a(3.0f, 4.0f);
    Vector2f b(1.0f, 2.0f);

    EXPECT_TRUE(a > b);
    EXPECT_FALSE(b > a);
}

TEST(Vector2Test, LessEqual)
{
    Vector2f a(1.0f, 2.0f);
    Vector2f b(1.0f, 2.0f);
    Vector2f c(3.0f, 4.0f);

    EXPECT_TRUE(a <= b);
    EXPECT_TRUE(a <= c);
    EXPECT_FALSE(c <= a);
}

TEST(Vector2Test, GreaterEqual)
{
    Vector2f a(1.0f, 2.0f);
    Vector2f b(1.0f, 2.0f);
    Vector2f c(3.0f, 4.0f);

    EXPECT_TRUE(a >= b);
    EXPECT_TRUE(c >= a);
    EXPECT_FALSE(a >= c);
}

TEST(Vector2Test, Addition)
{
    Vector2f a(1.0f, 2.0f);
    Vector2f b(3.0f, 4.0f);
    auto result = a + b;
    EXPECT_FLOAT_EQ(result.x, 4.0f);
    EXPECT_FLOAT_EQ(result.y, 6.0f);
}

TEST(Vector2Test, Subtraction)
{
    Vector2f a(5.0f, 6.0f);
    Vector2f b(1.0f, 2.0f);
    auto result = a - b;
    EXPECT_FLOAT_EQ(result.x, 4.0f);
    EXPECT_FLOAT_EQ(result.y, 4.0f);
}

TEST(Vector2Test, Multiplication)
{
    Vector2f a(2.0f, 3.0f);
    Vector2f b(4.0f, 5.0f);
    auto result = a * b;
    EXPECT_FLOAT_EQ(result.x, 8.0f);
    EXPECT_FLOAT_EQ(result.y, 15.0f);
}

TEST(Vector2Test, Division)
{
    Vector2f a(8.0f, 15.0f);
    Vector2f b(2.0f, 3.0f);
    auto result = a / b;
    EXPECT_FLOAT_EQ(result.x, 4.0f);
    EXPECT_FLOAT_EQ(result.y, 5.0f);
}

TEST(Vector2Test, ScalarMultiply)
{
    Vector2f a(2.0f, 3.0f);
    auto result = a * 4.0f;
    EXPECT_FLOAT_EQ(result.x, 8.0f);
    EXPECT_FLOAT_EQ(result.y, 12.0f);
}

TEST(Vector2Test, ScalarDivision)
{
    Vector2f a(8.0f, 12.0f);
    auto result = a / 4.0f;
    EXPECT_FLOAT_EQ(result.x, 2.0f);
    EXPECT_FLOAT_EQ(result.y, 3.0f);
}

TEST(Vector2Test, CompoundAdd)
{
    Vector2f a(1.0f, 2.0f);
    Vector2f b(3.0f, 4.0f);
    a += b;
    EXPECT_FLOAT_EQ(a.x, 4.0f);
    EXPECT_FLOAT_EQ(a.y, 6.0f);
}

TEST(Vector2Test, CompoundSubtract)
{
    Vector2f a(5.0f, 6.0f);
    Vector2f b(1.0f, 2.0f);
    a -= b;
    EXPECT_FLOAT_EQ(a.x, 4.0f);
    EXPECT_FLOAT_EQ(a.y, 4.0f);
}

TEST(Vector2Test, CompoundMultiply)
{
    Vector2f a(2.0f, 3.0f);
    Vector2f b(4.0f, 5.0f);
    a *= b;
    EXPECT_FLOAT_EQ(a.x, 8.0f);
    EXPECT_FLOAT_EQ(a.y, 15.0f);
}

TEST(Vector2Test, CompoundDivide)
{
    Vector2f a(8.0f, 15.0f);
    Vector2f b(2.0f, 3.0f);
    a /= b;
    EXPECT_FLOAT_EQ(a.x, 4.0f);
    EXPECT_FLOAT_EQ(a.y, 5.0f);
}

TEST(Vector2Test, AsConversion)
{
    Vector2f vf(1.5f, 2.5f);
    auto vi = vf.as<int>();
    EXPECT_EQ(vi.x, 1);
    EXPECT_EQ(vi.y, 2);
}

TEST(Vector2Test, Abs)
{
    Vector2f v(-3.0f, 4.0f);
    auto result = v.abs();
    EXPECT_FLOAT_EQ(result.x, 3.0f);
    EXPECT_FLOAT_EQ(result.y, 4.0f);
}

// ==================== Vector3 Tests ====================

TEST(Vector3Test, DefaultConstructor)
{
    Vector3f v;
    EXPECT_FLOAT_EQ(v.x, 0.0f);
    EXPECT_FLOAT_EQ(v.y, 0.0f);
    EXPECT_FLOAT_EQ(v.z, 0.0f);
}

TEST(Vector3Test, ParameterizedConstructor)
{
    Vector3f v(1.0f, 2.0f, 3.0f);
    EXPECT_FLOAT_EQ(v.x, 1.0f);
    EXPECT_FLOAT_EQ(v.y, 2.0f);
    EXPECT_FLOAT_EQ(v.z, 3.0f);
}

TEST(Vector3Test, CopyConstructor)
{
    Vector3f original(4.0f, 5.0f, 6.0f);
    Vector3f copy(original);
    EXPECT_FLOAT_EQ(copy.x, 4.0f);
    EXPECT_FLOAT_EQ(copy.y, 5.0f);
    EXPECT_FLOAT_EQ(copy.z, 6.0f);
}

TEST(Vector3Test, CopyAssignment)
{
    Vector3f a(1.0f, 2.0f, 3.0f);
    Vector3f b;
    b = a;
    EXPECT_FLOAT_EQ(b.x, 1.0f);
    EXPECT_FLOAT_EQ(b.y, 2.0f);
    EXPECT_FLOAT_EQ(b.z, 3.0f);
}

TEST(Vector3Test, MoveConstructor)
{
    Vector3f original(7.0f, 8.0f, 9.0f);
    Vector3f moved(std::move(original));
    EXPECT_FLOAT_EQ(moved.x, 7.0f);
    EXPECT_FLOAT_EQ(moved.y, 8.0f);
    EXPECT_FLOAT_EQ(moved.z, 9.0f);
}

TEST(Vector3Test, MoveAssignment)
{
    Vector3f a(10.0f, 11.0f, 12.0f);
    Vector3f b;
    b = std::move(a);
    EXPECT_FLOAT_EQ(b.x, 10.0f);
    EXPECT_FLOAT_EQ(b.y, 11.0f);
    EXPECT_FLOAT_EQ(b.z, 12.0f);
}

TEST(Vector3Test, Addition)
{
    Vector3f a(1.0f, 2.0f, 3.0f);
    Vector3f b(4.0f, 5.0f, 6.0f);
    auto result = a + b;
    EXPECT_FLOAT_EQ(result.x, 5.0f);
    EXPECT_FLOAT_EQ(result.y, 7.0f);
    EXPECT_FLOAT_EQ(result.z, 9.0f);
}

TEST(Vector3Test, GetType)
{
    Vector3f v;
    EXPECT_EQ(v.getType(), VectorType::vector3);
}

TEST(Vector3Test, SerializeDeserialize)
{
    Vector3f original(1.5f, -2.5f, 3.0f);
    auto bytes = original.serialize();

    EXPECT_EQ(bytes.size(), sizeof(float) * 3);

    auto deserialized = Vector3f::deserialize(bytes);
    EXPECT_FLOAT_EQ(deserialized.x, 1.5f);
    EXPECT_FLOAT_EQ(deserialized.y, -2.5f);
    EXPECT_FLOAT_EQ(deserialized.z, 3.0f);
}

TEST(Vector3Test, DeserializeWrongSizeThrows)
{
    nx_data tooSmall = {0x01, 0x02, 0x03};
    EXPECT_THROW(Vector3f::deserialize(tooSmall), std::runtime_error);
}

TEST(Vector3Test, SerializeDeserializeZero)
{
    Vector3f zero(0.0f, 0.0f, 0.0f);
    auto bytes = zero.serialize();
    auto result = Vector3f::deserialize(bytes);
    EXPECT_FLOAT_EQ(result.x, 0.0f);
    EXPECT_FLOAT_EQ(result.y, 0.0f);
    EXPECT_FLOAT_EQ(result.z, 0.0f);
}
