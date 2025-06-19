#include <nexilis/client/client_session.hh>

namespace nexilis::client
{

ClientSession::ClientSession(uint64_t id, ClientAPI* clientAPI)
    : BaseClient(id),
      m_clientAPI(clientAPI)
{
}

ClientSession::ClientSession(ClientSession&& other)
    : BaseClient(std::move(other)),
      m_clientAPI(std::move(other.m_clientAPI))
{
}

ClientSession& ClientSession::operator=(ClientSession&& other)
{
    if (this != &other)
    {
        BaseClient::operator=(std::move(other));
    }
    return *this;
}

bool operator==(const ClientSession& lhs, const ClientSession& rhs)
{
    return lhs.getId() == rhs.getId() &&
           lhs.getUsername() == rhs.getUsername();
}

} // namespace nexilis::client
