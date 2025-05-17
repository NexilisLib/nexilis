#include <nexilis/client/server_data.hh>

namespace nexilis::client
{

ServerData::ServerData(ServerData&& other)
    : m_password(std::move(other.m_password)),
      m_username(std::move(other.m_username)),
      m_inetUDPServerAddress(std::move(other.m_inetUDPServerAddress)),
      m_inetTCPServerAddress(std::move(other.m_inetTCPServerAddress)),
      m_boostTCPServerAddress(std::move(other.m_boostTCPServerAddress)),
      m_boostUDPServerAddress(std::move(other.m_boostUDPServerAddress)),
      m_unixDgramServerPath(std::move(other.m_unixDgramServerPath)),
      m_unixStreamServerPath(std::move(other.m_unixStreamServerPath))
{
}

ServerData::ServerData(const ServerData& other)
    : m_password(other.m_password),
      m_username(other.m_username),
      m_inetUDPServerAddress(other.m_inetUDPServerAddress),
      m_inetTCPServerAddress(other.m_inetTCPServerAddress),
      m_boostTCPServerAddress(other.m_boostTCPServerAddress),
      m_boostUDPServerAddress(other.m_boostUDPServerAddress),
      m_unixDgramServerPath(other.m_unixDgramServerPath),
      m_unixStreamServerPath(other.m_unixStreamServerPath)
{
}

ServerData& ServerData::operator=(ServerData&& other)
{
    if (this != &other)
    {
        m_password = std::move(other.m_password);
        m_username = std::move(other.m_username);
        m_inetUDPServerAddress = std::move(other.m_inetUDPServerAddress);
        m_inetTCPServerAddress = std::move(other.m_inetTCPServerAddress);
        m_boostTCPServerAddress = std::move(other.m_boostTCPServerAddress);
        m_boostUDPServerAddress = std::move(other.m_boostUDPServerAddress);
        m_unixDgramServerPath = std::move(other.m_unixDgramServerPath);
        m_unixStreamServerPath = std::move(other.m_unixStreamServerPath);
    }
    return *this;
}

ServerData& ServerData::operator=(const ServerData& other)
{
    if (this != &other)
    {
        m_password = other.m_password;
        m_username = other.m_username;
        m_inetUDPServerAddress = other.m_inetUDPServerAddress;
        m_inetTCPServerAddress = other.m_inetTCPServerAddress;
        m_boostTCPServerAddress = other.m_boostTCPServerAddress;
        m_boostUDPServerAddress = other.m_boostUDPServerAddress;
        m_unixDgramServerPath = other.m_unixDgramServerPath;
        m_unixStreamServerPath = other.m_unixStreamServerPath;
    }
    return *this;
}

} // namespace nexilis::client
