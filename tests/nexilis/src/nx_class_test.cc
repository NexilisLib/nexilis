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
