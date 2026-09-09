#include <nexilis/client/client_session.hh>
#include <nexilis/logger/file_log.hh>

namespace nexilis::client
{

ClientSession::ClientSession(uint64_t id, ClientAPI* clientAPI)
    : BaseClient(id),
      m_clientAPI(clientAPI)
{
}

ClientSession::ClientSession(ClientSession&& other) noexcept
    : BaseClient(std::move(other)),
      m_clientAPI(std::move(other.m_clientAPI))
{
}

ClientSession& ClientSession::operator=(ClientSession&& other) noexcept
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

Vector3f ClientSession::getPosition3D()
{
    auto pos = BaseClient::getObject3D().getPosition();
    std::stringstream ss;
    ss << "Position in ClientSessionCPP x: " << pos.x << " y:" << pos.y << " z:" << pos.z;
    FileLog::debug(ss.str());
    return pos;
}

void ClientSession::setPosition3D(float x, float y, float z)
{
    std::stringstream ss;
    ss << "Setting new position in ClientSessionCPP x: " << x << " y:" << y << " z:" << z;
    FileLog::debug(ss.str());
    BaseClient::getObject3D().setPosition(Vector3<float>(x, y, z));
}

} // namespace nexilis::client
