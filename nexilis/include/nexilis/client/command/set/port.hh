#ifndef NEXILIS_CLIENT_COMMAND_SET_PORT_HH
#define NEXILIS_CLIENT_COMMAND_SET_PORT_HH

#include <nexilis/client/base_api_command.hh>

#include <cstdint>
#include <string>

namespace nexilis::client
{

class SetPortCommand : public BaseAPICommand
{
public:
    SetPortCommand(std::string protocol, uint16_t port)
        : m_protocol(std::move(protocol)), m_port(port)
    {
    }

    ReadResult execute(ClientAPI& api, ClientAPI::ClientAPIData&) override
    {
        api.setProtocolPort(m_protocol, m_port);
        return ReadResult::success;
    }

private:
    std::string m_protocol;
    uint16_t m_port;
};

} // namespace nexilis::client

#endif
