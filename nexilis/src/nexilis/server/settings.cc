#include <nexilis/server/settings.hh>

namespace nexilis::server
{

void Settings::setPassphrase(const std::string& password)
{
    m_password = password;
}

bool Settings::isPassphrase(const std::string& password)
{
    return password == m_password;
}

bool Settings::hasPassphrase()
{
    return !m_password.empty();
}

void Settings::setRootPassword(const std::string& password)
{
    m_rootPassword = password;
}

bool Settings::isRootPassword(const std::string& password)
{
    return m_rootPassword == password;
}

void Settings::setTickrate(float tickrate)
{
    m_tickrate = tickrate;
}

} // namespace nexilis::server
