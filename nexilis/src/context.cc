#include "../include/nexilis/context.hh"

namespace nexilis
{

Context::Context(std::function<void()> sendMessage) : m_sendMessage(sendMessage)
{
}

}
