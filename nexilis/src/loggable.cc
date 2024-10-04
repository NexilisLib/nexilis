#include <nexilis/loggable.hh>

#include <sstream>

namespace nexilis
{

Loggable::Loggable(const std::string& name, const char* file)
    : m_name(name),
      m_file(std::string(file))
{
}

/// Move constructor.
Loggable::Loggable(Loggable&& other)
    : m_name(std::move(other.m_name)),
      m_file(std::move(other.m_file))
{
}

/// Move assignment operator.
Loggable& Loggable::operator=(Loggable&& other)
{
    if (this != &other)
    {
        m_name = std::move(other.m_name);
        m_file = std::move(other.m_file);
    }
    return *this;
}

void Loggable::debug(const std::string& message)
{
    printFromLogger(logger::LogLevel::DEBUG, createShortMessage(message));
}

void Loggable::debugExtra(const std::string& message, const int line)
{
    printFromLogger(logger::LogLevel::DEBUG, createLongMessage(message, line));
}

void Loggable::info(const std::string& message)
{
    printFromLogger(logger::LogLevel::INFO, createShortMessage(message));
}

void Loggable::infoExtra(const std::string& message, const int line)
{
    printFromLogger(logger::LogLevel::INFO, createLongMessage(message, line));
}

void Loggable::warning(const std::string& message)
{
    printFromLogger(logger::LogLevel::WARNING, createShortMessage(message));
}

void Loggable::warningExtra(const std::string& message, const int line)
{
    printFromLogger(logger::LogLevel::WARNING, createLongMessage(message, line));
}

void Loggable::error(const std::string& message)
{
    printFromLogger(logger::LogLevel::ERROR, createShortMessage(message));
}

void Loggable::errorExtra(const std::string& message, const int line)
{
    printFromLogger(logger::LogLevel::ERROR, createLongMessage(message, line));
}

void Loggable::critical(const std::string& message)
{
    printFromLogger(logger::LogLevel::CRITICAL, createShortMessage(message));
}

void Loggable::criticalExtra(const std::string& message, const int line)
{
    printFromLogger(logger::LogLevel::CRITICAL, createLongMessage(message, line));
}

std::string Loggable::createShortMessage(const std::string& message)
{
    std::stringstream ss;
    ss << m_name << ": " << message;
    return ss.str();
}

std::string Loggable::createLongMessage(const std::string& message, const int line)
{
    std::stringstream ss;
    ss << m_file << ":" << line << " " << message;
    return ss.str();
}

void Loggable::printFromLogger(logger::LogLevel logLevel, const std::string& message)
{
    switch (logLevel)
    {
        case logger::LogLevel::DEBUG:
        {
            Log::debug(message);
            break;
        }
        case logger::LogLevel::INFO:
        {
            Log::info(message);
            break;
        }
        case logger::LogLevel::WARNING:
        {
            Log::warning(message);
            break;
        }
        case logger::LogLevel::ERROR:
        {
            Log::error(message);
            break;
        }
        case logger::LogLevel::CRITICAL:
        {
            Log::critical(message);
            break;
        }
    }
}

} // namespace nexilis
