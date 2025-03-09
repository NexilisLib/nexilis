#include <gtest/gtest.h>

#include <nexilisc/logger/log_level_c.h>
#include <nexilisc/logger/logger_c.h>

#include <cstdlib>
#include <fstream>
#include <thread>

// Helper function to generate a random filename
std::string generateRandomFileName()
{
    return "test_file_" + std::to_string(rand()) + ".log";
}

// Test default values of the logger
TEST(LoggerTest_c, DefaultValues_c)
{
    nexilis_logger_LoggerC* logger = nexilis_logger_create();
    EXPECT_FALSE(nexilis_logger_get_level(logger, NEXILIS_LOGGER_LOGLEVEL_DEBUG));
    EXPECT_FALSE(nexilis_logger_get_level(logger, NEXILIS_LOGGER_LOGLEVEL_INFO));
    EXPECT_FALSE(nexilis_logger_get_level(logger, NEXILIS_LOGGER_LOGLEVEL_WARNING));
    EXPECT_FALSE(nexilis_logger_get_level(logger, NEXILIS_LOGGER_LOGLEVEL_ERROR));
    EXPECT_FALSE(nexilis_logger_get_level(logger, NEXILIS_LOGGER_LOGLEVEL_CRITICAL));
    nexilis_logger_destroy(logger);
}

TEST(LoggerTest_c, ConsoleHandler_c)
{
    nexilis_logger_LoggerC* logger = nexilis_logger_create();

    nexilis_logger_ConsoleHandler* console_handler = nexilis_logger_ConsoleHandler_create();

    // Redirect stdout to a buffer
    testing::internal::CaptureStdout();

    // Transfer ownership of the console handler to the logger
    nexilis_logger_add_console_handler(logger, console_handler);

    nexilis_logger_set_level(logger, NEXILIS_LOGGER_LOGLEVEL_ERROR);
    nexilis_logger_error(logger, "This is a test error message");

    // Get the captured stdout
    std::string output = testing::internal::GetCapturedStdout();

    // Check if the message was logged
    EXPECT_NE(output.find("ERROR: This is a test error message"), std::string::npos);

    // Clean up
    nexilis_logger_ConsoleHandler_destroy(console_handler);
    nexilis_logger_destroy(logger);
}

TEST(LoggerTest_c, FileHandler_c)
{
    nexilis_logger_LoggerC* logger = nexilis_logger_create();
    std::string fileName = generateRandomFileName();
    nexilis_logger_FileHandler* file_handler = nexilis_logger_FileHandler_create(fileName.c_str());

    nexilis_logger_add_file_handler(logger, file_handler);
    nexilis_logger_set_level(logger, NEXILIS_LOGGER_LOGLEVEL_INFO);
    nexilis_logger_info(logger, "Test message");

    // Read the file and check its contents.
    std::ifstream file(fileName);
    std::string line;
    std::getline(file, line);
    std::string expected = "INFO: Test message";

    EXPECT_EQ(expected, line);

    file.close();
    remove(fileName.c_str());

    nexilis_logger_FileHandler_destroy(file_handler);
    nexilis_logger_destroy(logger);
}

TEST(LoggerTest_c, FunctionHandler_c)
{
    nexilis_logger_LoggerC* logger = nexilis_logger_create();

    // Redirect stdout to a buffer.
    testing::internal::CaptureStdout();

    // Create function pointer handler.
    void (*handler)(const nexilis::logger::LogLevel&, const char*) = [](const nexilis::logger::LogLevel&, const char* msg)
    {
        std::cout << msg << std::endl;
    };

    nexilis_logger_FunctionHandler* function_handler = nexilis_logger_FunctionHandler_create(handler);

    // Transfer ownership of the function handler to the logger.
    nexilis_logger_add_function_handler(logger, function_handler);

    nexilis_logger_set_level(logger, NEXILIS_LOGGER_LOGLEVEL_INFO);
    nexilis_logger_info(logger, "Function handler test");

    // Get the captured stdout.
    std::string output = testing::internal::GetCapturedStdout();

    // Check if the message was logged.
    EXPECT_NE(output.find("INFO: Function handler test"), std::string::npos);

    // Clean up.
    nexilis_logger_FunctionHandler_destroy(function_handler);
    nexilis_logger_destroy(logger);
}

// Test multiple handlers with threads
TEST(LoggerTest_c, MultipleHandlersWithThreads_c)
{
    nexilis_logger_LoggerC* logger = nexilis_logger_create();
    std::string fileName = generateRandomFileName();
    nexilis_logger_ConsoleHandler* console_handler = nexilis_logger_ConsoleHandler_create();
    nexilis_logger_FileHandler* file_handler = nexilis_logger_FileHandler_create(fileName.c_str());

    nexilis_logger_add_console_handler(logger, console_handler);
    nexilis_logger_add_file_handler(logger, file_handler);
    nexilis_logger_set_level(logger, NEXILIS_LOGGER_LOGLEVEL_WARNING);

    std::thread t1([logger]()
                   { nexilis_logger_warning(logger, "Test message from thread 1"); });

    std::thread t2([logger]()
                   { nexilis_logger_warning(logger, "Test message from thread 2"); });

    t1.join();
    t2.join();

    // Check file output.
    std::ifstream file(fileName);
    std::string line;
    std::vector<std::string> lines;
    while (std::getline(file, line))
    {
        lines.push_back(line);
    }

    // Threads may start in wrong order.
    EXPECT_TRUE((lines[0] == "WARNING: Test message from thread 1" &&
                 lines[1] == "WARNING: Test message from thread 2") ||
                (lines[0] == "WARNING: Test message from thread 2" &&
                 lines[1] == "WARNING: Test message from thread 1"));

    file.close();
    remove(fileName.c_str());

    nexilis_logger_ConsoleHandler_destroy(console_handler);
    nexilis_logger_FileHandler_destroy(file_handler);
    nexilis_logger_destroy(logger);
}

// Parameterized test for setting and unsetting log levels
class LevelSetLogger_c : public ::testing::TestWithParam<nexilis_logger_loglevel>
{
protected:
    nexilis_logger_LoggerC* logger;

    void SetUp() override
    {
        logger = nexilis_logger_create();
    }

    void TearDown() override
    {
        nexilis_logger_destroy(logger);
    }
};

TEST_P(LevelSetLogger_c, SetLevel_c)
{
    nexilis_logger_loglevel level = GetParam();

    EXPECT_FALSE(nexilis_logger_get_level(logger, level));
    nexilis_logger_set_level(logger, level);
    EXPECT_TRUE(nexilis_logger_get_level(logger, level));
    nexilis_logger_unset_level(logger, level);
    EXPECT_FALSE(nexilis_logger_get_level(logger, level));
}

// Instantiate the test suite with all log levels
INSTANTIATE_TEST_SUITE_P(
        AllLogLevels,
        LevelSetLogger_c,
        ::testing::Values(
                NEXILIS_LOGGER_LOGLEVEL_DEBUG,
                NEXILIS_LOGGER_LOGLEVEL_INFO,
                NEXILIS_LOGGER_LOGLEVEL_WARNING,
                NEXILIS_LOGGER_LOGLEVEL_ERROR,
                NEXILIS_LOGGER_LOGLEVEL_CRITICAL));
