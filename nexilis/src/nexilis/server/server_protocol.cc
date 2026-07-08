#include <nexilis/server/server_protocol.hh>

namespace nexilis::server
{

ServerProtocol::ServerProtocol(const ServerConfig& settings)
    : m_command(settings)
{
}

ServerProtocol::ServerProtocol(ServerProtocol&& other)
    : m_messageHandler(std::move(other.m_messageHandler)),
      m_command(std::move(other.m_command)),
      m_activeConnections(other.m_activeConnections.load())
{
}

ServerProtocol& ServerProtocol::operator=(ServerProtocol&& other)
{
    if (this != &other)
    {
        m_messageHandler = std::move(other.m_messageHandler);
        m_command = std::move(other.m_command);
        m_activeConnections = std::move(other.m_activeConnections.load());
    }
    return *this;
}

} // namespace nexilis::server
