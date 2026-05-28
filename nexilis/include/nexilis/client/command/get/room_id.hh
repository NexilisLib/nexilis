#ifndef NEXILIS_CLIENT_COMMAND_GET_ROOM_ID_HH
#define NEXILIS_CLIENT_COMMAND_GET_ROOM_ID_HH

#include <nexilis/client/base_api_command.hh>

#include <mutex>

namespace nexilis::client
{

class GetRoomIdCommand : public BaseAPICommand
{
public:
    explicit GetRoomIdCommand(uint64_t room_id)
        : m_room_id(room_id)
    {
    }

    ReadResult execute(ClientAPI& api, ClientAPI::ClientAPIData& data) override
    {
        auto& mtx = data.getRoomsMutex();
        std::lock_guard<std::mutex> lock(*mtx);

        api.setRoomId(m_room_id);
        return ReadResult::success;
    }

private:
    uint64_t m_room_id;
};

} // namespace nexilis::client

#endif
