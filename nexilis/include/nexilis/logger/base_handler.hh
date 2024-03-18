#ifndef NEXILIS_LOGGER_BASE_HANDLER_HH
#define NEXILIS_LOGGER_BASE_HANDLER_HH

#include <nexilis/logger/log_level.hh>

#include <string>

namespace nexilis::logger
{

class BaseHandler
{
public:
    /// Destructor.
    virtual ~BaseHandler()
    {
    }

    // Overloading the equality operator.
    virtual bool operator==(const BaseHandler& other) const = 0;

    // Handle logs.
    // \param logLevel The log level of the message.
    // \param data The data of the given message.
    virtual void emit(LogLevel logLevel, const std::string& data) = 0;

};

} // namespace nexilis::logger

#endif
