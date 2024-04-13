#include <nexilis/client_storage.hh>
#include <nexilis/json.hh>
#include <nexilis/log.hh>
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

boost::json::object Json::getRoomData()
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

boost::json::object Json::getClientData()
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

boost::json::object Json::getServerData()
{
    boost::json::object serverDataObj;

    emplace(serverDataObj, getClientData());
    emplace(serverDataObj, getRoomData());

    return serverDataObj;
}

boost::json::object Json::getNexilisStatus(int status)
{
    boost::json::object nexilisStatusObj;
    nexilisStatusObj["nexilis_status"] = status;
    return nexilisStatusObj;
}

void Json::emplace(::boost::json::object& first, const ::boost::json::object& second)
{
    for (const auto& [key, value] : second)
    {
        first.emplace(key, value);
    }
}

boost::json::object Json::convertToJSON(const std::vector<uint8_t>& bytes)
{
    std::string jsonString(bytes.begin(), bytes.end());
    return boost::json::parse(jsonString).as_object();
}

void Json::print(const boost::json::object& obj)
{
    for (const auto& [key, value] : obj)
    {
        std::cout << key << ": ";

        if (value.is_string())
        {
            std::cout << value.as_string();
        }
        else if (value.is_number())
        {
            std::cout << value.as_int64();
        }
        else if (value.is_object())
        {
            // If the value is another object, recursively print it
            print(value.as_object());
        }
        // Very dirty hacks
        else if (value.is_array())
        {
            for (const auto& item : value.as_array())
            {
                if (item.is_string())
                {
                    std::cout << item.as_string();
                }
                else if (item.is_number())
                {
                    std::cout << item.as_int64();
                }
            }
        }
        else
        {
            Log::info("key: ", key);
            Log::info("value: ", value);
            Log::error("Unsupported value type");
        }
        std::cout << std::endl;
    }
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

void Json::saveToFile(const boost::json::object& obj, const std::string& filePath)
{
    std::ofstream file(filePath.c_str());
    if (!file.is_open())
    {
        Log::error("Failed to create a file at path: ", filePath);
    }
    file << boost::json::serialize(obj);
    file.close();
}

} // namespace nexilis
