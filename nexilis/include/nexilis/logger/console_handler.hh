#ifndef NEXILIS_LOGGER_CONSOLEHANDLER_HH
#define NEXILIS_LOGGER_CONSOLEHANDLER_HH

#include <nexilis/logger/base_handler.hh>

#include <iostream>

namespace nexilis
{

class ConsoleHandler : public BaseHandler
{
public:
    /// Constructor.
    ConsoleHandler() = default;

    /// Print messages to the console.
    /// \param logLevel The log level of the given message.
    /// \param data The data of the given message.
    void emit(const LogLevel& logLevel, const std::string& data) override
    {
        std::string color;
        switch (logLevel)
        {
            case LogLevel::DEBUG:
            case LogLevel::INFO:
                color = "\033[37m";
                break;
            case LogLevel::WARNING:
                color = "\033[33m";
                break;
            case LogLevel::ERROR:
            case LogLevel::CRITICAL:
                color = "\033[31m";
                break;
        }

        // Print colored data with ANSI escape code reset color.
        std::cout << color << data << "\033[0m" << std::endl;
    }

    // Overloading the equality operator.
    bool operator==(const ConsoleHandler& other) const
    {
        return this == &other;
    }
};

} // namespace nexilis

#endif
