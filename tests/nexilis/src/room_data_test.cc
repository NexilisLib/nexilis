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

#include <nexilis/room_data.hh>

using namespace nexilis;

TEST(RoomDataTest, ConstructorWithAllParams)
{
    RoomData room(100, "TestRoom", 42, RoomData::Context::_2D, 10);

    EXPECT_EQ(room.getCreatorId(), 100);
    EXPECT_EQ(room.getName(), "TestRoom");
    EXPECT_EQ(room.getId(), 42);
    EXPECT_EQ(room.getContext(), RoomData::Context::_2D);
    EXPECT_EQ(room.getMaxSize(), 10);
}

TEST(RoomDataTest, ConstructorDefaults)
{
    RoomData room(1, "MyRoom", 5);

    EXPECT_EQ(room.getCreatorId(), 1);
    EXPECT_EQ(room.getName(), "MyRoom");
    EXPECT_EQ(room.getId(), 5);
    EXPECT_EQ(room.getContext(), RoomData::Context::_3D);
    EXPECT_EQ(room.getMaxSize(), NEXILIS_DEFAULT_ROOM_CLIENT_AMOUNT);
}

TEST(RoomDataTest, CopyConstructor)
{
    RoomData original(10, "Original", 100, RoomData::Context::_2D, 5);
    RoomData copy(original);

    EXPECT_EQ(copy.getCreatorId(), 10);
    EXPECT_EQ(copy.getName(), "Original");
    EXPECT_EQ(copy.getId(), 100);
    EXPECT_EQ(copy.getContext(), RoomData::Context::_2D);
    EXPECT_EQ(copy.getMaxSize(), 5);
}

TEST(RoomDataTest, MoveConstructor)
{
    RoomData original(10, "Original", 100, RoomData::Context::_3D, 5);
    RoomData moved(std::move(original));

    EXPECT_EQ(moved.getCreatorId(), 10);
    EXPECT_EQ(moved.getName(), "Original");
    EXPECT_EQ(moved.getId(), 100);
    EXPECT_EQ(moved.getContext(), RoomData::Context::_3D);
    EXPECT_EQ(moved.getMaxSize(), 5);
}

TEST(RoomDataTest, CopyAssignment)
{
    RoomData a(1, "RoomA", 10, RoomData::Context::_2D, 3);
    RoomData b(2, "RoomB", 20, RoomData::Context::_3D, 7);

    b = a;

    EXPECT_EQ(b.getCreatorId(), 1);
    EXPECT_EQ(b.getName(), "RoomA");
    EXPECT_EQ(b.getId(), 10);
    EXPECT_EQ(b.getContext(), RoomData::Context::_2D);
    EXPECT_EQ(b.getMaxSize(), 3);
}

TEST(RoomDataTest, MoveAssignment)
{
    RoomData a(1, "RoomA", 10, RoomData::Context::_2D, 3);
    RoomData b(2, "RoomB", 20, RoomData::Context::_3D, 7);

    b = std::move(a);

    EXPECT_EQ(b.getCreatorId(), 1);
    EXPECT_EQ(b.getName(), "RoomA");
    EXPECT_EQ(b.getId(), 10);
    EXPECT_EQ(b.getContext(), RoomData::Context::_2D);
    EXPECT_EQ(b.getMaxSize(), 3);
}

TEST(RoomDataTest, CopyAssignmentSelf)
{
    RoomData room(1, "Self", 10);
    room = room;

    EXPECT_EQ(room.getName(), "Self");
    EXPECT_EQ(room.getId(), 10);
}

TEST(RoomDataTest, MoveAssignmentSelf)
{
    RoomData room(1, "SelfMove", 20);
    room = std::move(room);

    EXPECT_EQ(room.getName(), "SelfMove");
    EXPECT_EQ(room.getId(), 20);
}
