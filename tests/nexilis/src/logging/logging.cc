#include <gtest/gtest-param-test.h>
#include <gtest/gtest.h>

#include <nexilis/logger/log.hh>
#include <nexilis/logger/console_handler.hh>
#include <nexilis/logger/file_handler.hh>
#include <nexilis/logger/logger.hh>

bool logLevelsEmpty()
{
    return !nexilis::Log::getLevel(nexilis::logger::LogLevel::CRITICAL) &&
           !nexilis::Log::getLevel(nexilis::logger::LogLevel::DEBUG) &&
           !nexilis::Log::getLevel(nexilis::logger::LogLevel::WARNING) &&
           !nexilis::Log::getLevel(nexilis::logger::LogLevel::INFO) &&
           !nexilis::Log::getLevel(nexilis::logger::LogLevel::DEBUG);
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

INSTANTIATE_TEST_CASE_P(setLevelTests, SetLevelTest, testing::Values(nexilis::logger::LogLevel::CRITICAL, nexilis::logger::LogLevel::ERROR, nexilis::logger::LogLevel::WARNING, nexilis::logger::LogLevel::INFO, nexilis::logger::LogLevel::DEBUG));

TEST(LoggerTest, startStopLogging)
{
    nexilis::Log::startConsoleLogging();
    nexilis::Log::stopLogging();
    EXPECT_TRUE(logLevelsEmpty());
    EXPECT_TRUE(nexilis::Log::noHandlers());
}

// Test default state?
TEST(LoggerTest, testNoHandlers)
{
    EXPECT_TRUE(nexilis::Log::noHandlers());
}

// Test adding handlers

// Test removing handlers.
TEST(LoggerTest, removeHandlers)
{
    EXPECT_TRUE(nexilis::Log::noHandlers());

    auto handler = nexilis::logger::ConsoleHandler();
    uint64_t handlerId = handler.getId();

    nexilis::Log::addHandler(std::move(handler));
    EXPECT_FALSE(nexilis::Log::noHandlers());
    nexilis::Log::removeHandler(handlerId);
    EXPECT_TRUE(nexilis::Log::noHandlers());
}

TEST(LoggerTest, checkDefaultStartConsoleLogging)
{
    nexilis::Log::startConsoleLogging();
    EXPECT_TRUE(nexilis::Log::getLevel(nexilis::logger::LogLevel::CRITICAL));
    EXPECT_TRUE(nexilis::Log::getLevel(nexilis::logger::LogLevel::ERROR));
    EXPECT_TRUE(nexilis::Log::getLevel(nexilis::logger::LogLevel::WARNING));
    EXPECT_TRUE(nexilis::Log::getLevel(nexilis::logger::LogLevel::INFO));

    // Debug should be the only non active log by default.
    EXPECT_FALSE(nexilis::Log::getLevel(nexilis::logger::LogLevel::DEBUG));
    nexilis::Log::stopLogging();
}

TEST(LoggerTest, ConsoleHandler)
{
    nexilis::Log::startConsoleLogging();

    // Redirect console output to a stringstream.
    std::stringstream ss;
    std::streambuf* old_cout = std::cout.rdbuf();
    std::cout.rdbuf(ss.rdbuf());

    nexilis::Log::addHandler(nexilis::logger::ConsoleHandler());
    nexilis::Log::error("This is a test error message");

    // Reset cout's buffer to the original.
    std::cout.rdbuf(old_cout);

    // Check if the message was logged.
    std::string message = ss.str();
    EXPECT_NE(message.find("ERROR: This is a test error message"), std::string::npos);
    nexilis::Log::stopLogging();
}

TEST(LoggerTest, FileHandler)
{
    nexilis::Log::setLevel(nexilis::logger::LogLevel::INFO);
    std::string fileName = "test_file_" + std::to_string(rand()) + ".log";
    nexilis::Log::addHandler(nexilis::logger::FileHandler(fileName));

    nexilis::Log::info("Test message");

    std::ifstream file(fileName);
    std::string line;
    std::getline(file, line);
    std::string expected = "INFO: Test message";

    EXPECT_EQ(expected, line);

    file.close();
    std::remove(fileName.c_str());
    nexilis::Log::stopLogging();
}
