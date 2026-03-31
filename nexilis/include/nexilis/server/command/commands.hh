#ifndef NEXILIS_SERVER_COMMANDS_HH
#define NEXILIS_SERVER_COMMANDS_HH

#include <nexilis/server/command/command_result.hh>
#include <nexilis/server/user.hh>

namespace nexilis::server
{
class Commands
{
public:
    class DefaultArgs
    {
    public:
        DefaultArgs(User& user, Protocol& protocol, const nx_data& data, size_t messageId);

        User& getUser() const
        {
            return m_user;
        }
        Protocol& getProtocol() const
        {
            return m_protocol;
        }
        const nx_data& getData() const
        {
            return m_data;
        }
        size_t getMessageId() const
        {
            return m_messageId;
        }

    private:
        User& m_user;
        Protocol& m_protocol;
        nx_data m_data;
        size_t m_messageId;
    };

    class Set
    {
    public:
        class General
        {
        public:
            static CommandResult clientId(User& user, const nx_data& data);
            static CommandResult username(DefaultArgs args);
        };
    };
};
} // namespace nexilis::server

#endif
