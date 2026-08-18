#include <nexilis/protocol_manager.hh>
#include <nexilis/util.hh>

#include <algorithm>

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

bool ProtocolManager::hasProtocol(size_t id) const
{
    return std::any_of(m_items.begin(), m_items.end(),
                        [id](const ProtocolData& item) { return item.getId() == id; });
}

bool ProtocolManager::hasProtocolType(Protocol::Type type) const
{
    return std::any_of(m_items.begin(), m_items.end(),
                        [type](const ProtocolData& item) { return item.getType() == type; });
}

const ProtocolManager::ProtocolData* ProtocolManager::findById(size_t id) const
{
    auto it = std::find_if(m_items.begin(), m_items.end(),
                           [id](const ProtocolData& item) { return item.getId() == id; });
    return it != m_items.end() ? &(*it) : nullptr;
}

std::vector<const ProtocolManager::ProtocolData*> ProtocolManager::findByType(Protocol::Type type) const
{
    std::vector<const ProtocolData*> result;
    for (const auto& item : m_items)
    {
        if (item.getType() == type)
        {
            result.push_back(&item);
        }
    }
    return result;
}

} // namespace nexilis
