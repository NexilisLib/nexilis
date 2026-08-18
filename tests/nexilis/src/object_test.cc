#include <gtest/gtest.h>

#include <nexilis/object/object_2d.hh>
#include <nexilis/object/object_3d.hh>

using namespace nexilis;

// ==================== Object2D Tests ====================

TEST(Object2DTest, ConstructorAndGetters)
{
    Object2D obj(42, Vector2f(1.0f, 2.0f), Vector2f(3.0f, 4.0f));

    EXPECT_EQ(obj.getId(), 42);
    EXPECT_FLOAT_EQ(obj.getPosition().x, 1.0f);
    EXPECT_FLOAT_EQ(obj.getPosition().y, 2.0f);
    EXPECT_FLOAT_EQ(obj.getDimensions().x, 3.0f);
    EXPECT_FLOAT_EQ(obj.getDimensions().y, 4.0f);
}

TEST(Object2DTest, DefaultDimensions)
{
    Object2D obj(1, Vector2f(5.0f, 6.0f));

    Vector2f dims = obj.getDimensions();
    EXPECT_FLOAT_EQ(dims.x, 1.0f);
    EXPECT_FLOAT_EQ(dims.y, 1.0f);
}

TEST(Object2DTest, DefaultPosition)
{
    Object2D obj(1);

    Vector2f pos = obj.getPosition();
    EXPECT_FLOAT_EQ(pos.x, 0.0f);
    EXPECT_FLOAT_EQ(pos.y, 0.0f);
}

TEST(Object2DTest, MoveConstructor)
{
    Object2D original(10, Vector2f(1.0f, 2.0f));
    Object2D moved(std::move(original));

    EXPECT_EQ(moved.getId(), 10);
    EXPECT_FLOAT_EQ(moved.getPosition().x, 1.0f);
    EXPECT_FLOAT_EQ(moved.getPosition().y, 2.0f);
}

TEST(Object2DTest, MoveAssignment)
{
    Object2D a(1, Vector2f(1.0f, 1.0f));
    Object2D b(2, Vector2f(2.0f, 2.0f));

    b = std::move(a);

    EXPECT_EQ(b.getId(), 1);
    EXPECT_FLOAT_EQ(b.getPosition().x, 1.0f);
}

TEST(Object2DTest, SetPosition)
{
    Object2D obj(1, Vector2f(0.0f, 0.0f));
    obj.setPosition(Vector2f(5.0f, 10.0f));

    EXPECT_FLOAT_EQ(obj.getPosition().x, 5.0f);
    EXPECT_FLOAT_EQ(obj.getPosition().y, 10.0f);
}

TEST(Object2DTest, SetDimensions)
{
    Object2D obj(1);
    obj.setDimensions(Vector2f(2.0f, 3.0f));

    EXPECT_FLOAT_EQ(obj.getDimensions().x, 2.0f);
    EXPECT_FLOAT_EQ(obj.getDimensions().y, 3.0f);
}

TEST(Object2DTest, SetFilepath)
{
    Object2D obj(1);
    obj.setFilepath("/assets/sprite.png");

    EXPECT_EQ(obj.getFilepath(), "/assets/sprite.png");
}

TEST(Object2DTest, GetData)
{
    Object2D obj(42, Vector2f(1.0f, 2.0f), Vector2f(3.0f, 4.0f));
    auto data = obj.getData();

    EXPECT_FALSE(data.empty());
}

// ==================== Object3D Tests ====================

TEST(Object3DTest, ConstructorAndGetters)
{
    Object3D obj(99, Vector3f(1.0f, 2.0f, 3.0f), Vector3f(4.0f, 5.0f, 6.0f));

    EXPECT_EQ(obj.getId(), 99);
    EXPECT_FLOAT_EQ(obj.getPosition().x, 1.0f);
    EXPECT_FLOAT_EQ(obj.getPosition().y, 2.0f);
    EXPECT_FLOAT_EQ(obj.getPosition().z, 3.0f);
    EXPECT_FLOAT_EQ(obj.getDimensions().x, 4.0f);
    EXPECT_FLOAT_EQ(obj.getDimensions().y, 5.0f);
    EXPECT_FLOAT_EQ(obj.getDimensions().z, 6.0f);
}

TEST(Object3DTest, DefaultDimensions)
{
    Object3D obj(1, Vector3f(0.0f, 0.0f, 0.0f));

    Vector3f dims = obj.getDimensions();
    EXPECT_FLOAT_EQ(dims.x, 1.0f);
    EXPECT_FLOAT_EQ(dims.y, 1.0f);
    EXPECT_FLOAT_EQ(dims.z, 1.0f);
}

TEST(Object3DTest, MoveConstructor)
{
    Object3D original(10, Vector3f(1.0f, 2.0f, 3.0f));
    Object3D moved(std::move(original));

    EXPECT_EQ(moved.getId(), 10);
    EXPECT_FLOAT_EQ(moved.getPosition().x, 1.0f);
    EXPECT_FLOAT_EQ(moved.getPosition().y, 2.0f);
    EXPECT_FLOAT_EQ(moved.getPosition().z, 3.0f);
}

TEST(Object3DTest, MoveAssignment)
{
    Object3D a(1, Vector3f(1.0f, 1.0f, 1.0f));
    Object3D b(2, Vector3f(2.0f, 2.0f, 2.0f));

    b = std::move(a);

    EXPECT_EQ(b.getId(), 1);
    EXPECT_FLOAT_EQ(b.getPosition().x, 1.0f);
}

TEST(Object3DTest, SetPosition)
{
    Object3D obj(1);
    obj.setPosition(Vector3f(5.0f, 10.0f, 15.0f));

    EXPECT_FLOAT_EQ(obj.getPosition().x, 5.0f);
    EXPECT_FLOAT_EQ(obj.getPosition().y, 10.0f);
    EXPECT_FLOAT_EQ(obj.getPosition().z, 15.0f);
}

TEST(Object3DTest, SetDimensions)
{
    Object3D obj(1);
    obj.setDimensions(Vector3f(2.0f, 3.0f, 4.0f));

    EXPECT_FLOAT_EQ(obj.getDimensions().x, 2.0f);
    EXPECT_FLOAT_EQ(obj.getDimensions().y, 3.0f);
    EXPECT_FLOAT_EQ(obj.getDimensions().z, 4.0f);
}

TEST(Object3DTest, SetFilepath)
{
    Object3D obj(1);
    obj.setFilepath("/assets/model.obj");

    EXPECT_EQ(obj.getFilepath(), "/assets/model.obj");
}

TEST(Object3DTest, GetData)
{
    Object3D obj(42, Vector3f(1.0f, 2.0f, 3.0f), Vector3f(4.0f, 5.0f, 6.0f));
    auto data = obj.getData();

    EXPECT_FALSE(data.empty());
}
