#ifndef NEXILIS_LOGGER_BASE_HANDLER_HH
#define NEXILIS_LOGGER_BASE_HANDLER_HH

#include <nexilis/util.hh>
#include <nexilis/logger/log_level.hh>

#include <cstdint>
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

    uint64_t getId()
    {
        return m_id;
    }

private:
    uint64_t m_id = Util::getRandomUint64();
};

} // namespace nexilis::logger

#endif
