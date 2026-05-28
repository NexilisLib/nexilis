#ifndef NEXILIS_CLIENT_BASE_API_COMMAND_HH
#define NEXILIS_CLIENT_BASE_API_COMMAND_HH

#include <nexilis/client/client_api.hh>
#include <nexilis/client/read_result.hh>

namespace nexilis::client
{

class BaseAPICommand
{
public:
    virtual ~BaseAPICommand() = default;
    virtual ReadResult execute(ClientAPI& api, ClientAPI::ClientAPIData& data) = 0;
};

} // namespace nexilis::client

#endif
