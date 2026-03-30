#ifndef NEXILIS_SERVER_COMMANDS_HH
#define NEXILIS_SERVER_COMMANDS_HH

#include <nexilis/server/command/command_result.hh>
#include <nexilis/server/user.hh>

namespace nexilis::server
{
class Commands
{
public:
    Commands() = default;

    class Set
    {
    public:
        class General
        {
        public:
            static CommandResult clientId(User& user, const nx_data& data);
        };
    };
};
} // namespace nexilis::server

#endif
