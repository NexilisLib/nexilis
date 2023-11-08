#include <gtest/gtest-param-test.h>
#include <gtest/gtest.h>

#include <nexilis/log.hh>
#include <nexilis/logger/console_handler.hh>
#include <nexilis/logger/file_handler.hh>
#include <nexilis/logger/logger.hh>

bool logLevelsEmpty()
{
    return !nexilis::Log::getLevel(nexilis::LogLevel::CRITICAL) &&
           !nexilis::Log::getLevel(nexilis::LogLevel::DEBUG) &&
           !nexilis::Log::getLevel(nexilis::LogLevel::WARNING) &&
           !nexilis::Log::getLevel(nexilis::LogLevel::INFO) &&
           !nexilis::Log::getLevel(nexilis::LogLevel::DEBUG);
}

TEST(LoggerTest, loggerDefaultLogLevel)
{
    EXPECT_TRUE(logLevelsEmpty());
}

class SetLevelTest : public testing::TestWithParam<nexilis::LogLevel>
{
};

TEST_P(SetLevelTest, setLevel)
{
    nexilis::LogLevel level = GetParam();

    nexilis::Log::unsetLevel(level);
    EXPECT_FALSE(nexilis::Log::getLevel(level));
    nexilis::Log::setLevel(level);
    EXPECT_TRUE(nexilis::Log::getLevel(level));
    nexilis::Log::stopLogging();
    EXPECT_FALSE(nexilis::Log::getLevel(level));
}

INSTANTIATE_TEST_CASE_P(setLevelTests, SetLevelTest, testing::Values(nexilis::LogLevel::CRITICAL, nexilis::LogLevel::ERROR, nexilis::LogLevel::WARNING, nexilis::LogLevel::INFO, nexilis::LogLevel::DEBUG));

TEST(LoggerTest, startStopLogging)
{
    nexilis::Log::startConsoleLogging();
    nexilis::Log::stopLogging();
	EXPECT_TRUE(logLevelsEmpty());
}

TEST(LoggerTest, testNoHandlers)
{
    EXPECT_TRUE(nexilis::Log::noHandlers());
}

TEST(LoggerTest, checkDefaultStartConsoleLogging)
{
    nexilis::Log::startConsoleLogging();
    EXPECT_TRUE(nexilis::Log::getLevel(nexilis::LogLevel::CRITICAL));
    EXPECT_TRUE(nexilis::Log::getLevel(nexilis::LogLevel::ERROR));
    EXPECT_TRUE(nexilis::Log::getLevel(nexilis::LogLevel::WARNING));
    EXPECT_TRUE(nexilis::Log::getLevel(nexilis::LogLevel::INFO));

    // Debug should be the only non active log by default.
    EXPECT_FALSE(nexilis::Log::getLevel(nexilis::LogLevel::DEBUG));
}

TEST(LoggerTest, ConsoleHandler)
{
    nexilis::Log::startConsoleLogging();

    // Redirect console output to a stringstream.
    std::stringstream ss;
    std::streambuf* old_cout = std::cout.rdbuf();
    std::cout.rdbuf(ss.rdbuf());

    nexilis::Log::addHandler(nexilis::ConsoleHandler());
    nexilis::Log::error("This is a test error message");

    // Reset cout's buffer to the original.
    std::cout.rdbuf(old_cout);

    // Check if the message was logged.
    std::string message = ss.str();
    EXPECT_NE(message.find("ERROR: This is a test error message"), std::string::npos);
}

TEST(LoggerTest, FileHandler)
{
    std::string fileName = "test_file_" + std::to_string(rand()) + ".log";
    nexilis::Log::addHandler(nexilis::FileHandler(fileName));

    nexilis::Log::info("Test message");

    std::ifstream file(fileName);
    std::string line;
    std::getline(file, line);
    std::string expected = "INFO: Test message";

    EXPECT_EQ(expected, line);

    file.close();
    std::remove(fileName.c_str());
}
