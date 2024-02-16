#include <nexilis/protocol.hh>

namespace nexilis
{

Protocol::Protocol(Protocol&& other) :
    m_messageHandler(std::move(other.m_messageHandler))
{
}

Protocol& Protocol::operator=(Protocol&& other)
{
    if (this != &other)
    {
        m_messageHandler = std::move(other.m_messageHandler);
    }
    return *this;
}

}