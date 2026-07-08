#include <nexilis/server/server_config.hh>

namespace nexilis::server
{

void ServerConfig::setRootPassword(const std::string& password)
{
    m_rootPassword = password;
}

bool ServerConfig::isRootPassword(const std::string& password)
{
    return m_rootPassword == password;
}

bool ServerConfig::hasRootPassword()
{
    return !m_rootPassword.empty();
}

void ServerConfig::setTickrate(float tickrate)
{
    m_tickrate = tickrate;
}

} // namespace nexilis::server
