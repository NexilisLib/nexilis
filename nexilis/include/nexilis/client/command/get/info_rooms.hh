#ifndef NEXILIS_CLIENT_COMMAND_GET_INFO_ROOMS_HH
#define NEXILIS_CLIENT_COMMAND_GET_INFO_ROOMS_HH

#include <nexilis/client/base_api_command.hh>

#include <boost/json/value.hpp>

#include <mutex>
#include <string>

namespace nexilis::client
{

class GetInfoRoomsCommand : public BaseAPICommand
{
public:
    explicit GetInfoRoomsCommand(boost::json::value rooms)
        : m_rooms(std::move(rooms))
    {
    }

    static uint64_t toUint64(const boost::json::value& val)
    {
        if (val.is_uint64())
            return val.as_uint64();
        if (val.is_int64())
            return static_cast<uint64_t>(val.as_int64());
        return 0;
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

            uint64_t maxSize = toUint64(room.at("maxSize"));
            uint64_t context = toUint64(room.at("context"));
            uint64_t creatorId = toUint64(room.at("creatorId"));
            uint64_t id = toUint64(room.at("id"));

            std::vector<ClientSession> roomClients;

            if (room.as_object().contains("clients"))
            {
                auto clients = room.at("clients").as_array();

                for (const auto& client : clients)
                {
                    uint64_t client_id = toUint64(client.at("id"));

                    std::string username;
                    if (client.at("name").is_string())
                        username = client.at("name").as_string().c_str();

                    float object2DX = 0.0f;
                    if (client.at("roomPositionX").is_double())
                        object2DX = static_cast<float>(client.at("roomPositionX").as_double());

                    float object2DY = 0.0f;
                    if (client.at("roomPositionY").is_double())
                        object2DY = static_cast<float>(client.at("roomPositionY").as_double());

                    float dimension2DX = 0.0f;
                    if (client.at("roomDimensionX").is_double())
                        dimension2DX = static_cast<float>(client.at("roomDimensionX").as_double());

                    float dimension2DY = 0.0f;
                    if (client.at("roomDimensionY").is_double())
                        dimension2DY = static_cast<float>(client.at("roomDimensionY").as_double());

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
