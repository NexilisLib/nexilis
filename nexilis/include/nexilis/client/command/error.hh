#ifndef NEXILIS_CLIENT_COMMAND_ERROR_HH
#define NEXILIS_CLIENT_COMMAND_ERROR_HH

#include <nexilis/client/base_api_command.hh>

namespace nexilis::client
{

class ErrorCommand : public BaseAPICommand
{
public:
    explicit ErrorCommand(ReadResult result)
        : m_result(result)
    {
    }

    ReadResult execute(ClientAPI&, ClientAPI::ClientAPIData&) override
    {
        return m_result;
    }

private:
    ReadResult m_result;
};

} // namespace nexilis::client

#endif
