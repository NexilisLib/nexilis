#include <gtest/gtest.h>

#include <nexilis/logger/console_handler.hh>
#include <nexilis/logger/file_handler.hh>
#include <nexilis/logger/function_handler.hh>
#include <nexilis/logger/logger.hh>

#include <thread>

using namespace nexilis::logger;

TEST(LoggerTest, DefaultValues)
{
    Logger logger;
    EXPECT_FALSE(logger.getLevel(LogLevel::DEBUG));
    EXPECT_FALSE(logger.getLevel(LogLevel::INFO));
    EXPECT_FALSE(logger.getLevel(LogLevel::WARNING));
    EXPECT_FALSE(logger.getLevel(LogLevel::ERROR));
    EXPECT_FALSE(logger.getLevel(LogLevel::CRITICAL));
}

TEST(LoggerTest, ConsoleHandler)
{
    Logger logger;

    // Redirect console output to a stringstream.
    std::stringstream ss;
    std::streambuf* old_cout = std::cout.rdbuf();
    std::cout.rdbuf(ss.rdbuf());

    logger.addHandler(std::make_unique<ConsoleHandler>(ConsoleHandler()));
    logger.setLevel(LogLevel::ERROR);
    logger.error("This is a test error message");

    // Reset cout's buffer to the original.
    std::cout.rdbuf(old_cout);

    // Check if the message was logged.
    std::string message = ss.str();
    EXPECT_NE(message.find("ERROR: This is a test error message"), std::string::npos);
}

TEST(LoggerTest, FileHandler)
{
    Logger logger;
    std::string fileName = "test_file_" + std::to_string(rand()) + ".log";
    logger.addHandler(std::make_unique<FileHandler>(FileHandler(fileName)));
    logger.setLevel(LogLevel::INFO);

    logger.info("Test message");

    std::ifstream file(fileName);
    std::string line;
    std::getline(file, line);
    std::string expected = "INFO: Test message";

    EXPECT_EQ(expected, line);

    file.close();
    remove(fileName.c_str());
}

TEST(LoggerTest, FunctionHandler)
{
    Logger logger;

    // Redirect console output to a stringstream.
    std::stringstream ss;
    std::streambuf* old_cout = std::cout.rdbuf();
    std::cout.rdbuf(ss.rdbuf());

    auto handler = [](const LogLevel&, const std::string& msg)
    {
        std::cout << msg << std::endl;
    };
    logger.addHandler(std::make_unique<FunctionHandler>(FunctionHandler(handler)));
    logger.setLevel(LogLevel::INFO);

    logger.info("Function handler test");
    std::cout.rdbuf(old_cout);

    // Check if the message was logged.
    std::string message = ss.str();
    EXPECT_NE(message.find("INFO: Function handler test"), std::string::npos);
}

TEST(LoggerTest, MultipleHandlersWithThreads)
{
    Logger logger;
    std::string fileName = "test_file_" + std::to_string(rand()) + ".log";

    logger.addHandler(std::make_unique<ConsoleHandler>(ConsoleHandler()));
    logger.addHandler(std::make_unique<FileHandler>(FileHandler(fileName)));
    logger.setLevel(LogLevel::WARNING);

    std::thread t1([&logger]()
                   { logger.warning("Test message from thread 1"); });

    std::thread t2([&logger]()
                   { logger.warning("Test message from thread 2"); });

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
}

class LevelLogger
{
public:
    void setLevel(LogLevel level)
    {
        levels[static_cast<int>(level)] = true;
    }

    void unsetLevel(LogLevel level)
    {
        levels[static_cast<int>(level)] = false;
    }

    bool getLevel(LogLevel level) const
    {
        return levels[static_cast<int>(level)];
    }

private:
    bool levels[5] = {false};
};

// Define a parameterized test fixture
class LevelSetLogger : public ::testing::TestWithParam<LogLevel>
{
protected:
    Logger logger;
};

// Define the test using the parameterized fixture
TEST_P(LevelSetLogger, SetLevel)
{
    LogLevel level = GetParam();

    EXPECT_FALSE(logger.getLevel(level));
    logger.setLevel(level);
    EXPECT_TRUE(logger.getLevel(level));
    logger.unsetLevel(level);
    EXPECT_FALSE(logger.getLevel(level));
}

// Instantiate the test suite with all log levels
INSTANTIATE_TEST_SUITE_P(
        AllLogLevels,
        LevelSetLogger,
        ::testing::Values(
                LogLevel::DEBUG,
                LogLevel::INFO,
                LogLevel::WARNING,
                LogLevel::ERROR,
                LogLevel::CRITICAL));
