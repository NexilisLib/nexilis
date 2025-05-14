#include <nexilis/logger/logger.hh>

namespace nexilis::logger
{

void Logger::removeHandler(uint64_t handlerId)
{
    std::lock_guard<std::mutex> lock(m_mtx);

    auto it = std::remove_if(m_handlers.begin(), m_handlers.end(),
                             [&](const std::unique_ptr<BaseHandler>& handler)
                             { return handlerId == handler->getId(); });

    m_handlers.erase(it, m_handlers.end());
}

void Logger::clearHandlers()
{
    std::lock_guard<std::mutex> lock(m_mtx);
    for (auto& handler : m_handlers)
    {
        if (handler)
        {
            handler.reset();
        }
    }
    m_handlers.clear();
}

bool Logger::noHandlers()
{
    std::lock_guard<std::mutex> lock(m_mtx);
    return m_handlers.empty();
}

bool Logger::unsetLevel(const LogLevel& level)
{
    std::lock_guard<std::mutex> lock(m_mtx);
    const uint8_t mask = (1 << static_cast<uint8_t>(level));
    m_logLevel &= ~mask;
    return (m_logLevel & mask) == 0;
}

void Logger::unsetAllLevels()
{
    std::lock_guard<std::mutex> lock(m_mtx);
    m_logLevel = 0;
}

bool Logger::setLevel(const LogLevel& level)
{
    std::lock_guard<std::mutex> lock(m_mtx);
    return (m_logLevel |= (1 << static_cast<uint8_t>(level)));
}

void Logger::setAllLevels()
{
    std::lock_guard<std::mutex> lock(m_mtx);
    m_logLevel = static_cast<uint8_t>(-1);
}

bool Logger::getLevel(const LogLevel& level)
{
    std::lock_guard<std::mutex> lock(m_mtx);
    return (m_logLevel >> static_cast<uint8_t>(level)) & 1;
}

bool Logger::setMinimumLevel(const LogLevel& level)
{
    std::lock_guard<std::mutex> lock(m_mtx);
    return (m_logLevel |= (static_cast<uint8_t>(-1) << static_cast<uint8_t>(level)));
}

void Logger::setLogLevel(uint8_t logLevel)
{
    assert((std::is_same<decltype(logLevel), uint8_t>::value));
    std::lock_guard<std::mutex> lock(m_mtx);
    m_logLevel = logLevel;
}

} // namespace nexilis::logger
