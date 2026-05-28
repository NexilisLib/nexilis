#ifndef NEXILIS_CLIENT_COMMAND_SET_USERNAME_HH
#define NEXILIS_CLIENT_COMMAND_SET_USERNAME_HH

#include <nexilis/client/base_api_command.hh>

namespace nexilis::client
{

class SetUsernameCommand : public BaseAPICommand
{
public:
    explicit SetUsernameCommand(std::string& username)
        : m_username(username)
    {
    }

    ReadResult execute(ClientAPI& api, ClientAPI::ClientAPIData&) override
    {
        auto* client = api.getClientFromRoom(api.getClientId());
        if (client)
            client->setUsername(m_username);
        return ReadResult::success;
    }

private:
    std::string m_username;
};

} // namespace nexilis::client

#endif
