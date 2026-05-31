#ifndef NEXILIS_CLIENT_COMMAND_GET_CLIENT_ID_HH
#define NEXILIS_CLIENT_COMMAND_GET_CLIENT_ID_HH

#include <nexilis/client/base_api_command.hh>
#include <nexilis/client/packet.hh>

#include <mutex>

namespace nexilis::client
{

class GetClientIdCommand : public BaseAPICommand
{
public:
    explicit GetClientIdCommand(uint64_t client_id)
        : m_client_id(client_id)
    {
    }

    ReadResult execute(ClientAPI& api, ClientAPI::ClientAPIData& data) override
    {
        auto& mtx = data.getRoomsMutex();
        if (!mtx)
            return ReadResult::command_generation;
        std::lock_guard<std::mutex> lock(*mtx);

        api.setClientId(m_client_id);
        _Packet::_initialize(api);
        data.initialize();

        assert(api.getClientId() == m_client_id);
        assert(data.isInitialized());

        auto p_data = _Packet::clientIdentification();
        assert(p_data.size() >= 8);
        assert(Util::convertToType<uint64_t>({p_data.begin(), p_data.begin() + 8}) == api.getClientId());

        return ReadResult::success;
    }

private:
    uint64_t m_client_id;
};

} // namespace nexilis::client

#endif
