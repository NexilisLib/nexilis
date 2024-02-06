#include <nexilis/protocol.hh>

namespace nexilis
{

Protocol::Protocol() :
    m_port(static_cast<uint32_t>(-1))
{
}

Protocol::Protocol(uint32_t port)
    : m_port(port)
{
}

Protocol::Protocol(Protocol&& other) :
    m_port(std::move(other.m_port)),
    m_messageHandler(std::move(other.m_messageHandler))
{
}

Protocol& Protocol::operator=(Protocol&& other)
{
    if (this != &other)
    {
        m_port = std::move(other.m_port);
        m_messageHandler = std::move(other.m_messageHandler);
    }
    return *this;
}

}