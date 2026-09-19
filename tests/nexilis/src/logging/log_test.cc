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

#include <nexilis/logger/console_handler.hh>
#include <nexilis/logger/file_handler.hh>
#include <nexilis/logger/function_handler.hh>
#include <nexilis/logger/log.hh>
#include <nexilis/logger/logger.hh>

bool logLevelsEmpty()
{
    return !nexilis::Log::getLevel(nexilis::logger::LogLevel::Critical) &&
           !nexilis::Log::getLevel(nexilis::logger::LogLevel::Debug) &&
           !nexilis::Log::getLevel(nexilis::logger::LogLevel::Warning) &&
           !nexilis::Log::getLevel(nexilis::logger::LogLevel::Info) &&
           !nexilis::Log::getLevel(nexilis::logger::LogLevel::Debug);
}

TEST(LoggerTest, loggerDefaultLogLevel)
{
    EXPECT_TRUE(logLevelsEmpty());
}

class SetLevelTest : public testing::TestWithParam<nexilis::logger::LogLevel>
{
};

TEST_P(SetLevelTest, setLevel)
{
    nexilis::logger::LogLevel level = GetParam();

    nexilis::Log::unsetLevel(level);
    EXPECT_FALSE(nexilis::Log::getLevel(level));
    nexilis::Log::setLevel(level);
    EXPECT_TRUE(nexilis::Log::getLevel(level));
    nexilis::Log::stopLogging();
    EXPECT_FALSE(nexilis::Log::getLevel(level));
}

INSTANTIATE_TEST_CASE_P(setLevelTests, SetLevelTest, testing::Values(nexilis::logger::LogLevel::Critical, nexilis::logger::LogLevel::Error, nexilis::logger::LogLevel::Warning, nexilis::logger::LogLevel::Info, nexilis::logger::LogLevel::Debug));

TEST(LogTest, startStopLogging)
{
    nexilis::Log::startConsoleLogging();
    nexilis::Log::stopLogging();
    EXPECT_TRUE(logLevelsEmpty());
    EXPECT_TRUE(nexilis::Log::noHandlers());
}

// Test default state?
TEST(LogTest, testNoHandlers)
{
    EXPECT_TRUE(nexilis::Log::noHandlers());
}

// Test adding handlers

// Test removing handlers.
TEST(LogTest, removeHandlers)
{
    EXPECT_TRUE(nexilis::Log::noHandlers());

    auto handler = nexilis::logger::ConsoleHandler();
    uint64_t handlerId = handler.getId();

    nexilis::Log::addHandler(std::make_unique<nexilis::logger::ConsoleHandler>(handler));
    EXPECT_FALSE(nexilis::Log::noHandlers());
    nexilis::Log::removeHandler(handlerId);
    EXPECT_TRUE(nexilis::Log::noHandlers());
}

TEST(LogTest, checkDefaultStartConsoleLogging)
{
    nexilis::Log::startConsoleLogging();
    EXPECT_TRUE(nexilis::Log::getLevel(nexilis::logger::LogLevel::Critical));
    EXPECT_TRUE(nexilis::Log::getLevel(nexilis::logger::LogLevel::Error));
    EXPECT_TRUE(nexilis::Log::getLevel(nexilis::logger::LogLevel::Warning));
    EXPECT_TRUE(nexilis::Log::getLevel(nexilis::logger::LogLevel::Info));

    // Debug should be the only non active log by default.
    EXPECT_FALSE(nexilis::Log::getLevel(nexilis::logger::LogLevel::Debug));
    nexilis::Log::stopLogging();
}

TEST(LogTest, ConsoleHandler)
{
    nexilis::Log::startConsoleLogging();

    // Redirect console output to a stringstream.
    std::stringstream ss;
    std::streambuf* old_cout = std::cout.rdbuf();
    std::cout.rdbuf(ss.rdbuf());

    nexilis::Log::addHandler(std::make_unique<nexilis::logger::ConsoleHandler>(nexilis::logger::ConsoleHandler()));
    nexilis::Log::error("This is a test error message");

    // Reset cout's buffer to the original.
    std::cout.rdbuf(old_cout);

    // Check if the message was logged.
    std::string message = ss.str();
    EXPECT_NE(message.find("ERROR: This is a test error message"), std::string::npos);
    nexilis::Log::stopLogging();
}

TEST(LogTest, FileHandler)
{
    nexilis::Log::setLevel(nexilis::logger::LogLevel::Info);
    std::string filename = "test_file_" + std::to_string(rand()) + ".log";
    nexilis::Log::addHandler(std::make_unique<nexilis::logger::FileHandler>(nexilis::logger::FileHandler(filename)));

    nexilis::Log::info("Test message");

    std::ifstream file(filename);
    std::string line;
    std::getline(file, line);
    std::string expected = "INFO: Test message";

    EXPECT_EQ(expected, line);

    file.close();
    std::remove(filename.c_str());
    nexilis::Log::stopLogging();
}

TEST(LogTest, FunctionHandler)
{
    // Redirect console output to a stringstream.
    std::stringstream ss;
    std::streambuf* old_cout = std::cout.rdbuf();
    std::cout.rdbuf(ss.rdbuf());

    nexilis::Log::setLevel(nexilis::logger::LogLevel::Info);
    nexilis::Log::addHandler(std::make_unique<nexilis::logger::FunctionHandler>(nexilis::logger::FunctionHandler(
            [](const nexilis::logger::LogLevel&, const std::string& message)
            { std::cout << message << std::endl; })));

    nexilis::Log::info("Function handler test");

    // Reset cout's buffer to the original.
    std::cout.rdbuf(old_cout);

    // Check if the message was logged.
    std::string message = ss.str();
    EXPECT_NE(message.find("INFO: Function handler test"), std::string::npos);
    nexilis::Log::stopLogging();
}
