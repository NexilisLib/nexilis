#include <nexilis/loggable.hh>
#include <nexilis/log.hh>

#include <sstream>

namespace nexilis
{

Loggable::Loggable(const std::string& name) :
    m_name(name)
{
}

Loggable::Loggable(Loggable&& other) :
    m_name(std::move(other.m_name)),
    m_addLineNumber(std::move(other.m_addLineNumber)),
    m_addColon(std::move(other.m_addColon))
{
}

Loggable& Loggable::operator=(Loggable&& other)
{
    if (this != &other)
    {
        m_name = std::move(other.m_name);
        m_addLineNumber = std::move(other.m_addLineNumber);
        m_addColon = std::move(other.m_addColon);
    }
    return *this;
}

void Loggable::debug(const std::string& message)
{
    Log::debug(createMessage(message));
}

void Loggable::info(const std::string& message)
{
    Log::info(createMessage(message));
}

void Loggable::warning(const std::string& message)
{
    Log::info(createMessage(message));
}

void Loggable::error(const std::string& message)
{
    Log::info(createMessage(message));
}

void Loggable::critical(const std::string& message)
{
    Log::critical(createMessage(message));
}

std::string Loggable::createMessage(const std::string& text)
{
    std::string result;

    if (m_addLineNumber)
    {
        std::stringstream ss;
        ss << __FILE__ << ":" << std::dec << __LINE__ << std::endl;
        result += ss.str();
    }

    result += m_name;

    if (m_addColon)
    {
        result += ": ";
    }

    return result += text;
}

}
