#include <nexilis/server_protocol.hh>

namespace nexilis
{

ServerProtocol::ServerProtocol(ServerProtocol&& other) :
    m_messageHandler(std::move(other.m_messageHandler))
{
}

ServerProtocol& ServerProtocol::operator=(ServerProtocol&& other)
{
    if (this != &other)
    {
        m_messageHandler = std::move(other.m_messageHandler);
    }
    return *this;
}

}
