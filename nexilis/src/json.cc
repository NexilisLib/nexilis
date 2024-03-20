#include <nexilis/json.hh>
#include <nexilis/log.hh>
#include <nexilis/client_storage.hh>
#include <nexilis/room_storage.hh>

#include <boost/json/object.hpp>
#include <boost/json/value.hpp>

#include <fstream>

namespace nexilis
{

// Create JSON data from given key-value pairs
boost::json::object Json::createJSON(const std::map<std::string, boost::json::value>& keyValues)
{
    boost::json::object json_obj;
    for (const auto& kv : keyValues)
    {
        json_obj[kv.first] = kv.second;
    }
    return json_obj;
}

boost::json::object Json::getServerData()
{
    boost::json::object serverDataObj;
    serverDataObj["nexilis_status"] = 1;

    auto& clients = ClientStorage::getAllClients();

    if (clients.empty())
    {
        Log::info("Empty client data");
        serverDataObj["client_amount"] = 0;
    }
    else
    {
        serverDataObj["client_amount"] = clients.size();
        auto clientData = clientsToJSON(clients);
        serverDataObj["clients"] = std::move(clientData);
    }

    auto& rooms = RoomStorage::getAllRooms();

    if (rooms.empty())
    {
        Log::info("Empty room data");
        serverDataObj["room_amount"] = 0;
    }
    else
    {
        serverDataObj["room_amount"] = clients.size();
        auto roomData = roomsToJSON(rooms);
        serverDataObj["rooms"] = std::move(roomData);
    }
    return serverDataObj;
}

boost::json::array Json::clientsToJSON(const std::vector<Client>& clients)
{
    boost::json::array resultingArray;

    for (const auto& client : clients)
    {
        boost::json::object clientObj;
        if (client.getUsername().empty())
        {
            clientObj["username"] = "UNIDENTIFIED_CLIENT";
        }
        else
        {
            clientObj["username"] = client.getUsername();
        }
        clientObj["id"] = client.getId();
        clientObj["roomId"] = client.getRoomId();
        // ip address, port data. Add stuff if needed.
        resultingArray.emplace_back(std::move(clientObj));
    }
    return resultingArray;
}

boost::json::array Json::roomsToJSON(const std::vector<Room>& rooms)
{
    boost::json::array resultingArray;

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
        // Other room data.
        resultingArray.emplace_back(std::move(roomObj));
    }
    return resultingArray;
}

/// Read JSON data from file.
boost::json::value Json::readJSONFromFile(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        Log::error("Error: Unable to open file ", filename);
        return boost::json::value();
    }

    std::string json_str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    boost::json::error_code ec;
    boost::json::value json_value = boost::json::parse(json_str, ec);
    if (ec)
    {
        Log::error("Error parsing JSON: ", ec.message());
        return boost::json::value();
    }
    return json_value;
}

} // namespace nexilis
