#ifndef NEXILIS_SERVER_COMMANDS_HH
#define NEXILIS_SERVER_COMMANDS_HH

#include <nexilis/server/command/command_result.hh>
#include <nexilis/server/user.hh>
#include <nexilis/util.hh>

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

        friend std::ostream& operator<<(std::ostream& os, const DefaultArgs& obj)
        {
            os << obj.getUser().getId();

            if (!obj.m_user.getUsername().empty())
            {
                os << " [" << obj.getUser().getUsername() << "]";
            }

            os << ":" << Util::convertToString(obj.getData());

            os << "\n";
            return os;
        }

    private:
        User& m_user;
        Protocol& m_protocol;
        nx_data m_data;
        size_t m_messageId;
    };

    // SET
    class Set
    {
    public:
        class General
        {
        public:
            static CommandResult clientId(User& user, const nx_data& data);
            static CommandResult username(const DefaultArgs& args);
        };
        class Protocol
        {
        public:
            class BoostTCP
            {
            public:
                static CommandResult port(const DefaultArgs& args);
            };
        };
    };

    // GET
    class Get
    {
    public:
        class General
        {
        public:
            static CommandResult clientId(const DefaultArgs& args);
        };
    };

    class Room
    {
    public:
        class Management
        {
        public:
            static CommandResult join(const DefaultArgs& args);
            static CommandResult leave(const DefaultArgs& args);
            static CommandResult create(const DefaultArgs& args);
        };

        class Communicate
        {
        public:
            static CommandResult broadcast(const DefaultArgs& args);
        };

        class Player2D
        {
        public:
            static CommandResult position(const DefaultArgs& args);
            static CommandResult dimension(const DefaultArgs& args);
            static CommandResult movement(const DefaultArgs& args);
        };

        class Object2D
        {
        public:
            // static CommandResult
        };

        class Player3D
        {
        public:
        };

        class Object3D
        {
        public:
        };
    };
};

} // namespace nexilis::server

#endif
