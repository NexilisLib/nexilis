#include <nexilis/logger/logger.hh>

namespace nexilis::logger
{

void Logger::clearHandlers()
{
    m_handlers.clear();
}

bool Logger::noHandlers()
{
    return m_handlers.empty();
}

bool Logger::unsetLevel(const LogLevel& level)
{
    return (m_logLevel &= static_cast<uint8_t>(~(1 << static_cast<uint8_t>(level))));
}

bool Logger::setLevel(const LogLevel& level)
{
    return (m_logLevel |= (1 << static_cast<uint8_t>(level)));
}

bool Logger::getLevel(const LogLevel& level)
{
    return (m_logLevel >> static_cast<uint8_t>(level)) & 1;
}

bool Logger::setMinimumLevel(const LogLevel& level)
{
    return (m_logLevel |= (static_cast<uint8_t>(-1) << static_cast<uint8_t>(level)));
}

void Logger::setLogLevel(uint8_t logLevel)
{
    assert((std::is_same<decltype(logLevel), uint8_t>::value));
    m_logLevel = logLevel;
}

} // namespace nexilis::logger
