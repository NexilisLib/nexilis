#include <nexilis/server/server_protocol.hh>

namespace nexilis::server
{

ServerProtocol::ServerProtocol(const Settings& settings)
    : m_command(settings)
{
}

ServerProtocol::ServerProtocol(ServerProtocol&& other)
    : m_messageHandler(std::move(other.m_messageHandler)),
      m_command(std::move(other.m_command))
{
}

ServerProtocol& ServerProtocol::operator=(ServerProtocol&& other)
{
    if (this != &other)
    {
        m_messageHandler = std::move(other.m_messageHandler);
        m_command = std::move(other.m_command);
    }
    return *this;
}

} // namespace nexilis::server
