#ifndef NEXILIS_CLIENT_COMMAND_ROOM_GAMEITEM_HH
#define NEXILIS_CLIENT_COMMAND_ROOM_GAMEITEM_HH

#include <nexilis/client/base_api_command.hh>
#include <nexilis/client/client_api.hh>
#include <nexilis/object/game_item.hh>

namespace nexilis::client
{

class RoomGameItemCommand : public BaseAPICommand
{
public:
    RoomGameItemCommand(std::string action, uint64_t room_id, uint64_t item_id,
                        float x, float y, float z, float w, float h, float d,
                        const std::string& item_type = "",
                        const std::string& status = "",
                        const std::string& filepath = "")
        : m_action(action), m_room_id(room_id), m_item_id(item_id),
          m_x(x), m_y(y), m_z(z), m_w(w), m_h(h), m_d(d),
          m_item_type(item_type), m_status(status), m_filepath(filepath)
    {
    }

    ReadResult execute(ClientAPI&, ClientAPI::ClientAPIData& data) override
    {
        auto& mtx = data.getRoomsMutex();
        std::lock_guard<std::mutex> lock(*mtx);

        if (m_action == "create")
        {
            for (auto& room : data.getCurrentlyActiveRooms())
            {
                if (room.getId() == m_room_id)
                {
                    auto item = GameItem(m_item_id, m_item_type,
                                         {m_x, m_y, m_z}, {m_w, m_h, m_d},
                                         m_status, m_filepath);
                    room.addGameItem(std::move(item));
                    return ReadResult::success;
                }
            }
            return ReadResult::failure;
        }
        else if (m_action == "update")
        {
            for (auto& room : data.getCurrentlyActiveRooms())
            {
                if (room.getId() == m_room_id)
                {
                    room.updateGameItemStatus(m_item_id, m_status);
                    return ReadResult::success;
                }
            }
            return ReadResult::failure;
        }
        else if (m_action == "destroy")
        {
            for (auto& room : data.getCurrentlyActiveRooms())
            {
                if (room.getId() == m_room_id)
                {
                    room.deleteGameItem(m_item_id);
                    return ReadResult::success;
                }
            }
            return ReadResult::failure;
        }
        return ReadResult::not_found;
    }

private:
    std::string m_action;
    uint64_t m_room_id;
    uint64_t m_item_id;
    float m_x;
    float m_y;
    float m_z;
    float m_w;
    float m_h;
    float m_d;
    std::string m_item_type;
    std::string m_status;
    std::string m_filepath;
};

} // namespace nexilis::client

#endif
