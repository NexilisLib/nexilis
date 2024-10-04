#include <nexilis/base_client.hh>

#include <utility>

namespace nexilis
{

BaseClient::BaseClient(uint64_t id)
    : m_id(id)
{
}

BaseClient::BaseClient(BaseClient&& other)
    : m_id(std::move(other.m_id)),
      m_username(std::move(other.m_username)),
      m_object2D(std::move(other.m_object2D))
{
}

BaseClient& BaseClient::operator=(BaseClient&& other)
{
    if (this != &other)
    {
        m_id = std::move(other.m_id);
        m_username = std::move(other.m_username);
        m_object2D = std::move(other.m_object2D);
    }
    return *this;
}

} // namespace nexilis
