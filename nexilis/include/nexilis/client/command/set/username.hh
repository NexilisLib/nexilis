#ifndef NEXILIS_CLIENT_COMMAND_SET_USERNAME_HH
#define NEXILIS_CLIENT_COMMAND_SET_USERNAME_HH

#include <nexilis/client/base_api_command.hh>

namespace nexilis::client
{

class SetUsernameCommand : public BaseAPICommand
{
public:
    SetUsernameCommand(uint64_t client_id, std::string username)
        : m_client_id(client_id), m_username(std::move(username))
    {
    }

    ReadResult execute(ClientAPI& api, ClientAPI::ClientAPIData&) override
    {
        uint64_t id = m_client_id == 0 ? api.getClientId() : m_client_id;

        auto* client = api.getClientFromRoom(id);
        if (!client)
            return ReadResult::command_execution;

        client->setUsername(m_username);
        return ReadResult::success;
    }

private:
    uint64_t m_client_id;
    std::string m_username;
};

} // namespace nexilis::client

#endif
