#include <nexilis/nx_class.hh>

namespace nexilis
{

NxClass::NxClass(const std::string& name, const char* file)
    : m_classname(name),
      m_file(std::string(file)),
      m_logHeader(m_classname + ": ")
{
}

/// Move constructor.
NxClass::NxClass(NxClass&& other)
    : m_classname(std::move(other.m_classname)),
      m_file(std::move(other.m_file)),
      m_logHeader(std::move(other.m_logHeader))
{
}

/// Move assignment operator.
NxClass& NxClass::operator=(NxClass&& other)
{
    if (this != &other)
    {
        m_classname = std::move(other.m_classname);
        m_file = std::move(other.m_file);
        m_logHeader = std::move(other.m_logHeader);
    }
    return *this;
}

} // namespace nexilis
