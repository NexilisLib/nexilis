#include <nexilis/authentication.hh>

namespace nexilis
{

void Authentication::setPassphrase(const std::string& password)
{
    m_password = password;
}

bool Authentication::isPassphrase(const std::string& password)
{
    return password == m_password;
}

bool Authentication::hasPassphrase()
{
    return !m_password.empty();
}

void Authentication::setRootPassword(const std::string& password)
{
    m_rootPassword = password;
}

bool Authentication::isRootPassword(const std::string& password)
{
    return m_rootPassword == password;
}

void Authentication::setTickrate(float tickrate)
{
    m_tickrate = tickrate;
}

} // namespace nexilis
