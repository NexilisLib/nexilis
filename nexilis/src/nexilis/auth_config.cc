#include <nexilis/auth_config.hh>

namespace nexilis
{

AuthConfig::AuthConfig(AuthConfig&& other)
    : m_mode(std::move(other.m_mode)),
      m_password(std::move(other.m_password))
{
}

AuthConfig& AuthConfig::operator=(AuthConfig&& other)
{
    if (this != &other)
    {
        m_mode = std::move(other.m_mode);
        m_password = std::move(other.m_password);
    }
    return *this;
}

AuthConfig::AuthConfig(const AuthConfig& other)
    : m_mode(other.m_mode),
      m_password(other.m_password)
{
}

AuthConfig& AuthConfig::operator=(const AuthConfig& other)
{
    if (this != &other)
    {
        m_mode = other.m_mode;
        m_password = other.m_password;
    }
    return *this;
}

} // namespace nexilis
