#include <nexilis/server/user.hh>

namespace nexilis::server
{

User::User(uint64_t id, std::string ip_address) noexcept
    : BaseClient(id),
      m_ip_address(ip_address)
{
}

User::User(User&& other)
    : BaseClient(std::move(other)),
      m_ip_address(std::move(other.m_ip_address)),
      m_username(std::move(other.m_username)),
      m_roomId(std::move(other.m_roomId)),
      m_boostTCPSendToClient(std::move(other.m_boostTCPSendToClient)),
      m_hasRootAccess(std::move(other.m_hasRootAccess)),
      m_hasCommonAccess(std::move(other.m_hasCommonAccess))
{
}

/// Move assignment operator.
User& User::operator=(User&& other)
{
    if (this != &other)
    {
        BaseClient::operator=(std::move(other));
        m_ip_address = std::move(other.m_ip_address);
        m_username = std::move(other.m_username);
        m_roomId = std::move(other.m_roomId);
        m_boostTCPSendToClient = std::move(other.m_boostTCPSendToClient);
        m_hasRootAccess = std::move(other.m_hasRootAccess);
        m_hasCommonAccess = std::move(other.m_hasCommonAccess);
    }
    return *this;
}

bool User::boostTCPSend(const std::vector<uint8_t>& data)
{
    if (m_boostTCPSendToClient)
    {
        (m_boostTCPSendToClient)(data);
        return true;
    }
    return false;
}

bool User::boostUDPSend(const std::vector<uint8_t>& data)
{
    if (m_boostUDPSendToClient)
    {
        (m_boostUDPSendToClient)(data);
        return true;
    }
    return false;
}

bool User::unixStreamSend(const std::vector<uint8_t>& data)
{
    if (m_unixStreamSendToClient)
    {
        (m_boostTCPSendToClient)(data);
        return true;
    }
    return false;
}

} // namespace nexilis::server
