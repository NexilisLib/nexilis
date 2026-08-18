#include <gtest/gtest.h>

#include <nexilis/nx_class.hh>

using namespace nexilis;

TEST(NxClassTest, Constructs)
{
    NxClass cls("MyClass");
    EXPECT_EQ(cls.classname(), "MyClass");
    EXPECT_EQ(cls.header(), "nexilis::MyClass: ");
}

TEST(NxClassTest, ConstructsEmptyName)
{
    NxClass cls("");
    EXPECT_EQ(cls.classname(), "");
    EXPECT_EQ(cls.header(), "nexilis::: ");
}

TEST(NxClassTest, MoveConstructor)
{
    NxClass original("Original");
    NxClass moved(std::move(original));

    EXPECT_EQ(moved.classname(), "Original");
    EXPECT_EQ(moved.header(), "nexilis::Original: ");
}

TEST(NxClassTest, MoveAssignment)
{
    NxClass a("ClassA");
    NxClass b("ClassB");

    b = std::move(a);

    EXPECT_EQ(b.classname(), "ClassA");
    EXPECT_EQ(b.header(), "nexilis::ClassA: ");
}

TEST(NxClassTest, MoveAssignmentSelf)
{
    NxClass cls("SelfTest");
    cls = std::move(cls);

    EXPECT_EQ(cls.classname(), "SelfTest");
    EXPECT_EQ(cls.header(), "nexilis::SelfTest: ");
}
