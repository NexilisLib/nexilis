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

#include <nexilis/movement/movement_2D.hh>
#include <nexilis/movement/movement_3D.hh>
#include <nexilis/movement/movement_data.hh>

using namespace nexilis;

TEST(MovementDataTest, ConstructorAndGetters)
{
    nx_data msgData = {0x01, 0x02, 0x03};
    MovementData data(42, 0.016f, msgData, 7);

    EXPECT_EQ(data.getObjectId(), 42);
    EXPECT_FLOAT_EQ(data.getDeltatime(), 0.016f);
    EXPECT_EQ(data.getMessageData(), msgData);
    EXPECT_EQ(data.getMessageId(), 7);
}

TEST(MovementDataTest, EmptyMessageData)
{
    MovementData data(0, 0.0f, nx_data{}, 0);

    EXPECT_EQ(data.getObjectId(), 0);
    EXPECT_FLOAT_EQ(data.getDeltatime(), 0.0f);
    EXPECT_TRUE(data.getMessageData().empty());
    EXPECT_EQ(data.getMessageId(), 0);
}

TEST(Movement2DTest, ConstructorAndGetters)
{
    nx_data msgData = {0x01};
    MovementData params(10, 0.033f, msgData, 5);
    Vector2f amount(1.0f, 2.0f);
    auto func = [](double a, double b)
    { return a + b; };

    Movement2D movement(params, amount, func);

    EXPECT_EQ(movement.getType(), Movement<Vector2f>::Type::_2D);
    EXPECT_EQ(movement.getObjectId(), 10);
    EXPECT_FLOAT_EQ(movement.getDeltatime(), 0.033f);
    EXPECT_EQ(movement.getMessageId(), 5);

    Vector2f gotAmount = movement.getAmount();
    EXPECT_FLOAT_EQ(gotAmount.x, 1.0f);
    EXPECT_FLOAT_EQ(gotAmount.y, 2.0f);

    auto gotFunc = movement.getMovementFunc();
    EXPECT_DOUBLE_EQ(gotFunc(3.0, 4.0), 7.0);
}

TEST(Movement2DTest, ZeroAmount)
{
    MovementData params(0, 0.0f, nx_data{}, 0);
    Vector2f zero(0.0f, 0.0f);

    Movement2D movement(params, zero, [](double, double)
                        { return 0.0; });

    Vector2f amount = movement.getAmount();
    EXPECT_FLOAT_EQ(amount.x, 0.0f);
    EXPECT_FLOAT_EQ(amount.y, 0.0f);
}

TEST(Movement3DTest, ConstructorAndGetters)
{
    nx_data msgData = {0x01, 0x02};
    MovementData params(20, 0.05f, msgData, 9);
    Vector3f amount(1.0f, 2.0f, 3.0f);
    auto func = [](double a, double b)
    { return a * b; };

    Movement3D movement(params, amount, func);

    EXPECT_EQ(movement.getType(), Movement<Vector3f>::Type::_3D);
    EXPECT_EQ(movement.getObjectId(), 20);
    EXPECT_FLOAT_EQ(movement.getDeltatime(), 0.05f);
    EXPECT_EQ(movement.getMessageId(), 9);

    Vector3f gotAmount = movement.getAmount();
    EXPECT_FLOAT_EQ(gotAmount.x, 1.0f);
    EXPECT_FLOAT_EQ(gotAmount.y, 2.0f);
    EXPECT_FLOAT_EQ(gotAmount.z, 3.0f);

    auto gotFunc = movement.getMovementFunc();
    EXPECT_DOUBLE_EQ(gotFunc(5.0, 6.0), 30.0);
}

TEST(Movement3DTest, NegativeAmount)
{
    MovementData params(0, 0.0f, nx_data{}, 0);
    Vector3f amount(-1.0f, -2.0f, -3.0f);

    Movement3D movement(params, amount, [](double, double)
                        { return 0.0; });

    Vector3f gotAmount = movement.getAmount();
    EXPECT_FLOAT_EQ(gotAmount.x, -1.0f);
    EXPECT_FLOAT_EQ(gotAmount.y, -2.0f);
    EXPECT_FLOAT_EQ(gotAmount.z, -3.0f);
}
