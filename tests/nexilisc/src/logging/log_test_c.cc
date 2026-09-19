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

#include <gtest/gtest-param-test.h>
#include <gtest/gtest.h>

#include <nexilisc/logger/log_c.h>

#include <fstream>

bool logLevelsEmpty()
{
    return !nexilis_log_get_level(NEXILIS_LOGGER_LOGLEVEL_CRITICAL) &&
           !nexilis_log_get_level(NEXILIS_LOGGER_LOGLEVEL_ERROR) &&
           !nexilis_log_get_level(NEXILIS_LOGGER_LOGLEVEL_WARNING) &&
           !nexilis_log_get_level(NEXILIS_LOGGER_LOGLEVEL_INFO) &&
           !nexilis_log_get_level(NEXILIS_LOGGER_LOGLEVEL_DEBUG);
}

TEST(LogTest_c, loggerDefaultLogLevel)
{
    EXPECT_TRUE(logLevelsEmpty());
}

// Test setting and unsetting log levels
TEST(LogTest_c, SetLevel_c)
{
    EXPECT_FALSE(nexilis_log_get_level(NEXILIS_LOGGER_LOGLEVEL_DEBUG));
    EXPECT_TRUE(nexilis_log_set_level(NEXILIS_LOGGER_LOGLEVEL_DEBUG));
    EXPECT_TRUE(nexilis_log_get_level(NEXILIS_LOGGER_LOGLEVEL_DEBUG));
    EXPECT_TRUE(nexilis_log_unset_level(NEXILIS_LOGGER_LOGLEVEL_DEBUG));
    EXPECT_FALSE(nexilis_log_get_level(NEXILIS_LOGGER_LOGLEVEL_DEBUG));
}

// Test setting and unsetting all log levels
TEST(LogTest_c, SetAllLevels_c)
{
    EXPECT_FALSE(nexilis_log_get_level(NEXILIS_LOGGER_LOGLEVEL_DEBUG));
    EXPECT_FALSE(nexilis_log_get_level(NEXILIS_LOGGER_LOGLEVEL_INFO));
    EXPECT_FALSE(nexilis_log_get_level(NEXILIS_LOGGER_LOGLEVEL_WARNING));
    EXPECT_FALSE(nexilis_log_get_level(NEXILIS_LOGGER_LOGLEVEL_ERROR));
    EXPECT_FALSE(nexilis_log_get_level(NEXILIS_LOGGER_LOGLEVEL_CRITICAL));

    nexilis_log_set_all_levels();

    EXPECT_TRUE(nexilis_log_get_level(NEXILIS_LOGGER_LOGLEVEL_DEBUG));
    EXPECT_TRUE(nexilis_log_get_level(NEXILIS_LOGGER_LOGLEVEL_INFO));
    EXPECT_TRUE(nexilis_log_get_level(NEXILIS_LOGGER_LOGLEVEL_WARNING));
    EXPECT_TRUE(nexilis_log_get_level(NEXILIS_LOGGER_LOGLEVEL_ERROR));
    EXPECT_TRUE(nexilis_log_get_level(NEXILIS_LOGGER_LOGLEVEL_CRITICAL));

    nexilis_log_unset_all_levels();

    EXPECT_FALSE(nexilis_log_get_level(NEXILIS_LOGGER_LOGLEVEL_DEBUG));
}

class SetLevelTest : public testing::TestWithParam<nexilis_logger_loglevel>
{
};

TEST_P(SetLevelTest, setLevel)
{
    nexilis_logger_loglevel level = GetParam();

    nexilis_log_unset_level(level);
    EXPECT_FALSE(nexilis_log_get_level(level));
    nexilis_log_set_level(level);
    EXPECT_TRUE(nexilis_log_get_level(level));
    nexilis_log_stop_logging();
    EXPECT_FALSE(nexilis_log_get_level(level));
}

INSTANTIATE_TEST_CASE_P(setLevelTests, SetLevelTest, testing::Values(NEXILIS_LOGGER_LOGLEVEL_CRITICAL, NEXILIS_LOGGER_LOGLEVEL_ERROR, NEXILIS_LOGGER_LOGLEVEL_WARNING, NEXILIS_LOGGER_LOGLEVEL_INFO, NEXILIS_LOGGER_LOGLEVEL_DEBUG));

TEST(LogTest_c, startStopLogging)
{
    nexilis_log_start_console_logging(NEXILIS_LOGGER_LOGLEVEL_DEBUG);
    nexilis_log_stop_logging();
    EXPECT_TRUE(logLevelsEmpty());
    EXPECT_TRUE(nexilis_log_no_handlers());
}

TEST(LogTest_c, testNoHandlers)
{
    EXPECT_TRUE(nexilis_log_no_handlers());
}

TEST(LogTest_c, removeHandlers)
{
    EXPECT_TRUE(nexilis_log_no_handlers());

    auto handler = nexilis_log_add_console_handler();
    EXPECT_FALSE(nexilis_log_no_handlers());
    nexilis_log_remove_handler(handler);
    EXPECT_TRUE(nexilis_log_no_handlers());
}

TEST(LogTest_c, startConsoleLogging)
{
    nexilis_log_start_console_logging(NEXILIS_LOGGER_LOGLEVEL_INFO);
    EXPECT_TRUE(nexilis_log_get_level(NEXILIS_LOGGER_LOGLEVEL_CRITICAL));
    EXPECT_TRUE(nexilis_log_get_level(NEXILIS_LOGGER_LOGLEVEL_ERROR));
    EXPECT_TRUE(nexilis_log_get_level(NEXILIS_LOGGER_LOGLEVEL_WARNING));
    EXPECT_TRUE(nexilis_log_get_level(NEXILIS_LOGGER_LOGLEVEL_INFO));

    EXPECT_FALSE(nexilis_log_get_level(NEXILIS_LOGGER_LOGLEVEL_DEBUG));
    nexilis_log_stop_logging();
}

TEST(LogTest_c, ConsoleHandler)
{
    nexilis_log_start_console_logging(NEXILIS_LOGGER_LOGLEVEL_DEBUG);

    // Redirect console output to a stringstream.
    std::stringstream ss;
    std::streambuf* old_cout = std::cout.rdbuf();
    std::cout.rdbuf(ss.rdbuf());

    nexilis_log_add_console_handler();
    nexilis_log_error("This is a test error message");

    // Reset cout's buffer to the original.
    std::cout.rdbuf(old_cout);

    // Check if the message was logged.
    std::string message = ss.str();
    EXPECT_NE(message.find("ERROR: This is a test error message"), std::string::npos);
    nexilis_log_stop_logging();
}

TEST(LogTest_c, FileHandler)
{
    nexilis_log_set_level(NEXILIS_LOGGER_LOGLEVEL_INFO);
    std::string filename = "test_file_" + std::to_string(rand()) + ".log";

    nexilis_log_add_file_handler(filename.c_str());
    nexilis_log_info("Test message");

    std::ifstream file(filename);
    std::string line;
    std::getline(file, line);
    std::string expected = "INFO: Test message";

    EXPECT_EQ(expected, line);

    file.close();
    std::remove(filename.c_str());
    nexilis_log_stop_logging();
}

TEST(LogTest_c, FunctionHandler)
{
    nexilis_log_set_level(NEXILIS_LOGGER_LOGLEVEL_INFO);

    // Redirect console output to a stringstream.
    std::stringstream ss;
    std::streambuf* old_cout = std::cout.rdbuf();
    std::cout.rdbuf(ss.rdbuf());

    auto handler = [](const nexilis_logger_loglevel&, const char* msg)
    {
        std::cout << msg << std::endl;
    };

    nexilis_log_add_function_handler(handler);
    nexilis_log_info("Function handler test");

    // Reset cout's buffer to the original.
    std::cout.rdbuf(old_cout);

    // Check if the message was logged.
    std::string message = ss.str();
    EXPECT_NE(message.find("INFO: Function handler test"), std::string::npos);

    // Clean up.
    nexilis_log_stop_logging();
}
