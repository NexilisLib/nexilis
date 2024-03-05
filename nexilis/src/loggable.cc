#include <nexilis/loggable.hh>

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

}
