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

#include <cstring>

TEST(NxDataTest_c, Create)
{
    nx_data_c data = nexilis_nx_data_create(10);
    EXPECT_NE(data.data, nullptr);
    EXPECT_EQ(nexilis_nx_data_get_size(&data), 10u);

    nexilis_nx_data_destroy(&data);
    EXPECT_EQ(data.data, nullptr);
}

TEST(NxDataTest_c, CreateFrom)
{
    const uint8_t source[] = {0x01, 0x02, 0x03, 0x04};
    nx_data_c data = nexilis_nx_data_create_from(source, 4);
    EXPECT_NE(data.data, nullptr);
    EXPECT_EQ(nexilis_nx_data_get_size(&data), 4u);

    const uint8_t* retrieved = nexilis_nx_data_get_data(&data);
    EXPECT_EQ(retrieved[0], 0x01);
    EXPECT_EQ(retrieved[1], 0x02);
    EXPECT_EQ(retrieved[2], 0x03);
    EXPECT_EQ(retrieved[3], 0x04);

    nexilis_nx_data_destroy(&data);
}

TEST(NxDataTest_c, CreateZeroSize)
{
    nx_data_c data = nexilis_nx_data_create(0);
    EXPECT_NE(data.data, nullptr);
    EXPECT_EQ(nexilis_nx_data_get_size(&data), 0u);

    nexilis_nx_data_destroy(&data);
}

TEST(NxDataTest_c, GetMutable)
{
    nx_data_c data = nexilis_nx_data_create(4);
    uint8_t* mutable_ptr = nexilis_nx_data_get_mutable(&data);
    ASSERT_NE(mutable_ptr, nullptr);

    mutable_ptr[0] = 0xAA;
    mutable_ptr[1] = 0xBB;
    mutable_ptr[2] = 0xCC;
    mutable_ptr[3] = 0xDD;

    const uint8_t* const_ptr = nexilis_nx_data_get_data(&data);
    EXPECT_EQ(const_ptr[0], 0xAA);
    EXPECT_EQ(const_ptr[1], 0xBB);
    EXPECT_EQ(const_ptr[2], 0xCC);
    EXPECT_EQ(const_ptr[3], 0xDD);

    nexilis_nx_data_destroy(&data);
}

TEST(NxDataTest_c, GetDataNullSafe)
{
    nx_data_c data = {nullptr};
    EXPECT_EQ(nexilis_nx_data_get_size(&data), 0u);
    EXPECT_EQ(nexilis_nx_data_get_data(&data), nullptr);
}
