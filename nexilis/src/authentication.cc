#include <nexilis/authentication.hh>

namespace nexilis
{

void Authentication::setRootPassword(const std::string& password)
{
    assert(!password.empty());
    m_rootPassword = password;
}

bool Authentication::isRootPassword(const std::string& password)
{
    assert(!m_rootPassword.empty());
    return m_rootPassword == password;
}

void Authentication::setCommonPassword(const std::string& password)
{
    assert(!password.empty());
    m_password = password;
}

bool Authentication::isCommonPassword(const std::string& password)
{
    assert(!m_password.empty());
    return password == m_password;
}

}