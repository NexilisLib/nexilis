#ifndef NEXILIS_LOGGER_CONSOLE_HANDLER_HH
#define NEXILIS_LOGGER_CONSOLE_HANDLER_HH

#include <nexilis/logger/base_handler.hh>

#include <iostream>

namespace nexilis::logger
{

class ConsoleHandler : public BaseHandler
{
public:
    /// Constructor.
    ConsoleHandler() = default;

    /// Print messages to the console.
    /// \param logLevel The log level of the given message.
    /// \param data The data of the given message.
    void emit(LogLevel logLevel, const std::string& data) override;

    // Overloading the equality operator.
    bool operator==(const BaseHandler& other) const override
    {
        return this == &other;
    }
};

} // namespace nexilis

#endif
