#include <nexilis/base_client.hh>

#include <utility>

namespace nexilis
{

BaseClient::BaseClient(uint64_t id) :
    m_id(id)
{
}

BaseClient::BaseClient(BaseClient&& other) :
    m_id(std::move(other.m_id)),
    m_username(std::move(other.m_username))
{
}

BaseClient& BaseClient::operator=(BaseClient&& other)
{
    if (this != &other)
    {
        m_id = std::move(other.m_id);
        m_username = std::move(other.m_username);
    }
    return *this;
}


}
