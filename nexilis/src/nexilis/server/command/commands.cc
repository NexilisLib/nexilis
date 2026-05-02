#include <nexilis/server/command/command.hh>
#include <nexilis/server/command/commands.hh>

namespace nexilis::server
{

DefaultArgs::DefaultArgs(User& user, Protocol& protocol, const nx_data& data, size_t messageId)
    : m_user(user),
      m_protocol(protocol),
      m_data(data),
      m_messageId(messageId)
{
}

} // namespace nexilis::server
