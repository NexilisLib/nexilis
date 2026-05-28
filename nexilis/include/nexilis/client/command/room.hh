#ifndef NEXILIS_CLIENT_COMMAND_ROOM_HH
#define NEXILIS_CLIENT_COMMAND_ROOM_HH

#include <nexilis/client/base_api_command.hh>
#include <nexilis/client/client_api.hh>

namespace nexilis::client
{

class RoomCommand : public BaseAPICommand
{
public:
    RoomCommand() = default;

    ReadResult execute(ClientAPI&, ClientAPI::ClientAPIData&) override
    {
        // Base class implementation, actual functionality will be in specialized commands
        return ReadResult::not_implemented;
    }
};

} // namespace nexilis::client

#endif
