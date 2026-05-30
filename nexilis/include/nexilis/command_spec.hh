#ifndef NEXILIS_COMMAND_SPEC_HH
#define NEXILIS_COMMAND_SPEC_HH

#include <nexilis/client/command_parser.hh>
#include <nexilis/server/command/command.hh>
#include <nexilis/server/command/command_result.hh>
#include <nexilis/server/command/commands.hh>

namespace nexilis
{

/// Aggregate that composes all command-related objects from both the client
/// and server sides.
class CommandSpec
{
public:
    /// Minimal concrete Protocol for testing.
    class TestProtocol : public Protocol
    {
    public:
        void start() override
        {
        }
        void stop() override
        {
        }
        Type getType() override
        {
            return Type::UNKNOWN;
        }
    };

    // Server-side state (declaration order = init order)
    server::Settings settings;
    TestProtocol protocol;
    server::User user{1, "127.0.0.1"};
    server::Command command{settings};

    // Client-side state
    client::ServerData serverData;
    client::ClientAPI clientApi{serverData};

    /// Pack server-side args from raw command bytes.
    server::DefaultArgs args(const nx_data& data, size_t messageId = 0)
    {
        return server::DefaultArgs(user, protocol, data, messageId);
    }

    /// Shorthand for server-side command dispatch.
    server::CommandResult read(const nx_data& data, size_t messageId = 0)
    {
        return command.read(data, user, protocol, messageId);
    }

    /// Parse a JSON object into a client-side command handler.
    static std::unique_ptr<client::BaseAPICommand> parse(const boost::json::object& json)
    {
        return client::CommandParser::parse(json);
    }
};

} // namespace nexilis

#endif
