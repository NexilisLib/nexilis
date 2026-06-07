#ifndef NEXILIS_CLIENT_COMMAND_GET_INFO_ROOMS_HH
#define NEXILIS_CLIENT_COMMAND_GET_INFO_ROOMS_HH

#include <nexilis/client/base_api_command.hh>

namespace nexilis::client
{

class GetInfoRoomsCommand : public BaseAPICommand
{
public:
    explicit GetInfoRoomsCommand(boost::json::value rooms)
        : m_rooms(std::move(rooms))
    {
    }

    ReadResult execute(ClientAPI& api, ClientAPI::ClientAPIData& data) override
    {
        if (!m_rooms.is_array())
        {
            return ReadResult::invalid_input;
        }

        auto rooms = m_rooms.as_array();
        std::vector<Room> newRooms;
        for (const auto& room : rooms)
        {
            std::string name;
            if (room.at("name").is_string())
                name = room.at("name").as_string().c_str();

            uint64_t maxSize = Util::toUint64(room.at("max_size"));
            uint64_t context = Util::toUint64(room.at("context"));
            uint64_t creatorId = Util::toUint64(room.at("creator_id"));
            uint64_t id = Util::toUint64(room.at("room_id"));

            std::vector<ClientSession> roomClients;

            if (room.as_object().contains("clients"))
            {
                auto clients = room.at("clients").as_array();

                for (const auto& client : clients)
                {
                    uint64_t client_id = Util::toUint64(client.at("id"));

                    std::string username;
                    if (client.at("name").is_string())
                        username = client.at("name").as_string().c_str();

                    float object2DX = 0.0f;
                    if (client.at("x").is_double())
                        object2DX = static_cast<float>(client.at("x").as_double());

                    float object2DY = 0.0f;
                    if (client.at("y").is_double())
                        object2DY = static_cast<float>(client.at("y").as_double());

                    float dimension2DX = 0.0f;
                    if (client.at("width").is_double())
                        dimension2DX = static_cast<float>(client.at("width").as_double());

                    float dimension2DY = 0.0f;
                    if (client.at("height").is_double())
                        dimension2DY = static_cast<float>(client.at("height").as_double());

                    ClientSession newClient(client_id, &api);
                    newClient.getObject2D().setPosition({object2DX, object2DY});
                    newClient.getObject2D().setDimensions({dimension2DX, dimension2DY});
                    newClient.setUsername(username);
                    roomClients.emplace_back(std::move(newClient));
                }
            }

            auto roomData = RoomData(creatorId, name, id,
                                     static_cast<RoomData::Context>(context), maxSize);
            newRooms.emplace_back(Room(roomData, std::move(roomClients)));
        }

        auto& mtx = data.getRoomsMutex();
        if (mtx)
        {
            std::lock_guard<std::mutex> lock(*mtx);
            data.setCurrentlyActiveRooms(std::move(newRooms));
        }

        return ReadResult::success;
    }

private:
    boost::json::value m_rooms;
};

} // namespace nexilis::client

#endif
