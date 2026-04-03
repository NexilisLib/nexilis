#include <nexilis/json.hh>
#include <nexilis/logger/log.hh>
#include <nexilis/server/client_storage.hh>
#include <nexilis/server/room_storage.hh>
#include <nexilis/server/server_json.hh>

#include <boost/json/parse.hpp>
#include <boost/json/serialize.hpp>

namespace nexilis::server
{

boost::json::object ServerJson::getRoomData()
{
    boost::json::object roomDataObj;

    auto& rooms = RoomStorage::getAllRooms();

    if (rooms.empty())
    {
        Log::info("Empty room data");
        roomDataObj["room_amount"] = 0;
    }
    else
    {
        roomDataObj["room_amount"] = rooms.size();
        auto roomData = roomsToJSON(rooms);
        roomDataObj["rooms"] = std::move(roomData);
    }
    return roomDataObj;
}

boost::json::object ServerJson::getClientData()
{
    boost::json::object clientDataObj;

    auto& clients = ClientStorage::getAllClients();

    if (clients.empty())
    {
        Log::info("Empty client data");
        clientDataObj["client_amount"] = 0;
    }
    else
    {
        clientDataObj["client_amount"] = clients.size();
        auto clientData = clientsToJSON(clients);
        clientDataObj["clients"] = std::move(clientData);
    }
    return clientDataObj;
}

boost::json::object ServerJson::getServerData()
{
    boost::json::object serverDataObj;

    Json::emplace(serverDataObj, getClientData());
    Json::emplace(serverDataObj, getRoomData());

    return serverDataObj;
}

boost::json::array ServerJson::clientsToJSON(const std::vector<std::unique_ptr<User>>& clients)
{
    boost::json::array resultingArray;

    for (const auto& client : clients)
    {
        boost::json::object clientObj;
        if (client->getUsername().empty())
        {
            clientObj["username"] = "UNIDENTIFIED_CLIENT";
        }
        else
        {
            clientObj["username"] = client->getUsername();
        }
        clientObj["id"] = client->getId();
        clientObj["roomId"] = client->getRoomId();
        resultingArray.emplace_back(std::move(clientObj));
    }
    return resultingArray;
}

boost::json::array ServerJson::roomsToJSON(const std::vector<Room>& rooms)
{
    boost::json::array resultingArray;
    resultingArray.reserve(rooms.size());

    for (const auto& room : rooms)
    {
        boost::json::object roomObj;

        if (room.getName().empty())
        {
            roomObj["name"] = "UNIDENTIFIED_ROOM";
        }
        else
        {
            roomObj["name"] = room.getName();
        }

        roomObj["maxSize"] = room.getMaxSize();
        roomObj["id"] = room.getId();
        roomObj["creatorId"] = room.getCreatorId();
        roomObj["context"] = static_cast<uint32_t>(room.getContext());

        // Get data from clients in a room.
        boost::json::array clientArray;
        clientArray.reserve(room.getClients().size());

        auto& roomClients = room.getClients();

        for (auto& client : roomClients)
        {
            auto* clientPointer = ClientStorage::getClientById(client);

            if (clientPointer)
            {
                boost::json::object clientObj;
                clientObj["id"] = clientPointer->getId();

                if (clientPointer->getUsername().empty())
                {
                    Log::warning("Client has empty username!");
                    clientObj["name"] = "";
                }
                else
                {
                    clientObj["name"] = clientPointer->getUsername();
                }

                auto position2D = clientPointer->getObject2D().getPosition();
                clientObj["roomPositionX"] = std::move(position2D.x);
                clientObj["roomPositionY"] = std::move(position2D.y);

                auto dimension2D = clientPointer->getObject2D().getDimensions();
                clientObj["roomDimensionX"] = std::move(dimension2D.x);
                clientObj["roomDimensionY"] = std::move(dimension2D.y);

                clientArray.emplace_back(std::move(clientObj));
            }
        }

        if (roomClients.size() > 0)
        {
            roomObj["clients"] = std::move(clientArray);
        }
        resultingArray.emplace_back(std::move(roomObj));
    }
    return resultingArray;
}

} // namespace nexilis::server
