#include <nexilis/protocol_manager.hh>
#include <nexilis/util.hh>

namespace nexilis
{

ProtocolManager::ProtocolData::ProtocolData(Protocol::Type type)
    : m_type(type),
      m_id(Util::getRandomUint64())
{
}

ProtocolManager::ProtocolData::ProtocolData(const ProtocolData& other)
    : m_type(other.m_type),
      m_id(other.m_id)
{
}

ProtocolManager::ProtocolData::ProtocolData(ProtocolData&& other)
    : m_type(std::move(other.m_type)),
      m_id(std::move(other.m_id))
{
}

ProtocolManager::ProtocolData& ProtocolManager::ProtocolData::operator=(const ProtocolData& other)
{
    if (this != &other)
    {
        m_type = other.m_type;
        m_id = other.m_id;
    }
    return *this;
}

/// Move assignment operator.
ProtocolManager::ProtocolData& ProtocolManager::ProtocolData::operator=(ProtocolData&& other)
{
    if (this != &other)
    {
        m_type = std::move(other.m_type);
        m_id = std::move(other.m_id);
    }
    return *this;
}

} // namespace nexilis
