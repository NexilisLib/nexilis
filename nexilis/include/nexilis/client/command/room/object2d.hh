#ifndef NEXILIS_CLIENT_COMMAND_ROOM_OBJECT2D_HH
#define NEXILIS_CLIENT_COMMAND_ROOM_OBJECT2D_HH

#include <nexilis/client/base_api_command.hh>
#include <nexilis/client/client_api.hh>

namespace nexilis::client
{

class RoomObject2DCommand : public BaseAPICommand
{
public:
    RoomObject2DCommand(std::string action, uint64_t room_id, uint64_t object_id, float x, float y, float w, float h, const std::string& filepath = "")
        : m_action(action), m_room_id(room_id), m_object_id(object_id), m_x(x), m_y(y), m_w(w), m_h(h), m_filepath(filepath)
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
                    auto object = Object2D(m_object_id, {m_x, m_y}, {m_w, m_h});
                    object.setFilepath(m_filepath);
                    room.addObject(std::move(object));
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
                    room.deleteObject2D(m_object_id);
                    return ReadResult::success;
                }
            }
            return ReadResult::failure;
        }
        else if (m_action == "move")
        {
            for (auto& room : data.getCurrentlyActiveRooms())
            {
                if (room.getId() == m_room_id)
                {
                    auto object = room.getObject2DById(m_object_id);
                    if (object)
                    {
                        object->setPosition({m_x, m_y});
                        return ReadResult::success;
                    }
                }
            }
            return ReadResult::failure;
        }
        else if (m_action == "create_moving")
        {
            // Both createMoving and createMovingTest use same logic
            for (auto& room : data.getCurrentlyActiveRooms())
            {
                if (room.getId() == m_room_id)
                {
                    auto object = Object2D(m_object_id, {m_x, m_y}, {m_w, m_h});
                    object.setFilepath(m_filepath);
                    room.addObject(std::move(object));
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
    uint64_t m_object_id;
    float m_x;
    float m_y;
    float m_w;
    float m_h;
    std::string m_filepath;
};

} // namespace nexilis::client

#endif
