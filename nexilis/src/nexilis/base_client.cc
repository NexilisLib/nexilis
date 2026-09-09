#include <nexilis/base_client.hh>

#include <utility>

namespace nexilis
{

BaseClient::BaseClient(uint64_t id)
    : m_id(id),
      m_object2D(id),
      m_object3D(id)
{
}

BaseClient::BaseClient(BaseClient&& other) noexcept
    : m_id(std::move(other.m_id)),
      m_username(std::move(other.m_username)),
      m_object2D(std::move(other.m_object2D)),
      m_object3D(std::move(other.m_object3D))
{
}

BaseClient& BaseClient::operator=(BaseClient&& other) noexcept
{
    if (this != &other)
    {
        m_id = std::move(other.m_id);
        m_username = std::move(other.m_username);
        m_object2D = std::move(other.m_object2D);
        m_object3D = std::move(other.m_object3D);
    }
    return *this;
}

} // namespace nexilis
