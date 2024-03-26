#include <nexilis/client.hh>

namespace nexilis
{

Client::Client(std::string ip_address) noexcept
    : m_ip_address(ip_address)
{
}

Client::Client(Client&& other)
    : m_ip_address(std::move(other.m_ip_address)),
      m_username(std::move(other.m_username)),
      m_id(std::move(other.m_id)),
      m_roomId(std::move(other.m_roomId)),
      m_boostTCPSendToClient(std::move(other.m_boostTCPSendToClient)),
      m_hasRootAccess(std::move(other.m_hasRootAccess)),
      m_hasCommonAccess(std::move(other.m_hasCommonAccess))
{
}

/// Move assignment operator.
Client& Client::operator=(Client&& other)
{
    if (this != &other)
    {
        m_ip_address = std::move(other.m_ip_address);
        m_username = std::move(other.m_username);
        m_id = std::move(other.m_id);
        m_roomId = std::move(other.m_roomId);
        m_boostTCPSendToClient = std::move(other.m_boostTCPSendToClient);
        m_hasRootAccess = std::move(other.m_hasRootAccess);
        m_hasCommonAccess = std::move(other.m_hasCommonAccess);
    }
    return *this;
}

bool Client::boostTCPSend(std::vector<uint8_t> data)
{
    if (m_boostTCPSendToClient)
    {
        (*m_boostTCPSendToClient)(std::move(data));
        return true;
    }
    return false;
}



}
