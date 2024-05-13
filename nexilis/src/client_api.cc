#include <nexilis/room_storage.hh>
#include <nexilis/client_api.hh>
#include <nexilis/common/util.hh>
#include <nexilis/json.hh>
#include <nexilis/packet.hh>
#include <nexilis/log.hh>

namespace nexilis
{

// ClientAPI::ServerData
ClientAPI::ServerData::ServerData(const std::string& password)
    : m_password(password)
{
}

ClientAPI::ServerData::ServerData(const std::string password, const std::string username)
    : m_password(password),
      m_username(username)
{
}

ClientAPI::ServerData::ServerData(ServerData&& other)
    : m_password(std::move(other.m_password)),
      m_username(std::move(other.m_username)),
      m_inetUDPServerAddress(std::move(other.m_inetUDPServerAddress)),
      m_inetUDPPort(std::move(other.m_inetUDPPort)),
      m_inetTCPServerAddress(std::move(other.m_inetTCPServerAddress)),
      m_inetTCPPort(std::move(other.m_inetTCPPort)),
      m_boostTCPServerAddress(std::move(other.m_boostTCPServerAddress)),
      m_boostTCPServerPort(std::move(other.m_boostTCPServerPort)),
      m_boostUDPServerAddress(std::move(other.m_boostUDPServerAddress)),
      m_boostUDPServerPort(std::move(other.m_boostUDPServerPort)),
      m_unixDgramServerPath(std::move(other.m_unixDgramServerPath)),
      m_unixStreamServerPath(std::move(other.m_unixStreamServerPath))
{
}

ClientAPI::ServerData::ServerData(const ServerData& other)
    : m_password(other.m_password),
      m_username(other.m_username),
      m_inetUDPServerAddress(other.m_inetUDPServerAddress),
      m_inetUDPPort(other.m_inetUDPPort),
      m_inetTCPServerAddress(other.m_inetTCPServerAddress),
      m_inetTCPPort(other.m_inetTCPPort),
      m_boostTCPServerAddress(other.m_boostTCPServerAddress),
      m_boostTCPServerPort(other.m_boostTCPServerPort),
      m_boostUDPServerAddress(other.m_boostUDPServerAddress),
      m_boostUDPServerPort(other.m_boostUDPServerPort),
      m_unixDgramServerPath(other.m_unixDgramServerPath),
      m_unixStreamServerPath(other.m_unixStreamServerPath)
{
}

ClientAPI::ServerData& ClientAPI::ServerData::operator=(ServerData&& other)
{
    if (this != &other)
    {
        m_password = std::move(other.m_password);
        m_username = std::move(other.m_username);
        m_inetUDPServerAddress = std::move(other.m_inetUDPServerAddress);
        m_inetUDPPort = std::move(other.m_inetUDPPort);
        m_inetTCPServerAddress = std::move(other.m_inetTCPServerAddress);
        m_inetTCPPort = std::move(other.m_inetTCPPort);
        m_boostTCPServerAddress = std::move(other.m_boostTCPServerAddress);
        m_boostTCPServerPort = std::move(other.m_boostTCPServerPort);
        m_boostUDPServerAddress = std::move(other.m_boostUDPServerAddress);
        m_boostUDPServerPort = std::move(other.m_boostUDPServerPort);
        m_unixDgramServerPath = std::move(other.m_unixDgramServerPath);
        m_unixStreamServerPath = std::move(other.m_unixStreamServerPath);
    }
    return *this;
}

ClientAPI::ServerData& ClientAPI::ServerData::operator=(const ServerData& other)
{
    if (this != &other)
    {
        m_password = other.m_password;
        m_username = other.m_username;
        m_inetUDPServerAddress = other.m_inetUDPServerAddress;
        m_inetUDPPort = other.m_inetUDPPort;
        m_inetTCPServerAddress = other.m_inetTCPServerAddress;
        m_inetTCPPort = other.m_inetTCPPort;
        m_boostTCPServerAddress = other.m_boostTCPServerAddress;
        m_boostTCPServerPort = other.m_boostTCPServerPort;
        m_boostUDPServerAddress = other.m_boostUDPServerAddress;
        m_boostUDPServerPort = other.m_boostUDPServerPort;
        m_unixDgramServerPath = other.m_unixDgramServerPath;
        m_unixStreamServerPath = other.m_unixStreamServerPath;
    }
    return *this;
}

// ClientAPI::Room
ClientAPI::Room::Room(const Room& other) :
    m_name(other.m_name),
    m_creatorId(other.m_creatorId),
    m_roomId(other.m_roomId),
    m_maxSize(other.m_maxSize),
    m_clients(other.m_clients),
    m_roomMessages(other.m_roomMessages)
{
}

ClientAPI::Room::Room(Room&& other) :
    m_name(std::move(other.m_name)),
    m_creatorId(std::move(other.m_creatorId)),
    m_roomId(std::move(other.m_roomId)),
    m_maxSize(std::move(other.m_maxSize)),
    m_clients(std::move(other.m_clients)),
    m_roomMessages(std::move(other.m_roomMessages))
{
}

ClientAPI::Room& ClientAPI::Room::operator=(const Room& other)
{
    if (this != &other)
    {
        m_name = other.m_name;
        m_creatorId = other.m_creatorId;
        m_roomId = other.m_roomId;
        m_maxSize = other.m_maxSize;
        m_clients = other.m_clients;
        m_roomMessages = other.m_roomMessages;
    }
    return *this;
}

ClientAPI::Room& ClientAPI::Room::operator=(Room&& other)
{
    if (this != &other)
    {
        m_name = std::move(other.m_name);
        m_creatorId = std::move(other.m_creatorId);
        m_roomId = std::move(other.m_roomId);
        m_maxSize = std::move(other.m_maxSize);
        m_clients = std::move(other.m_clients);
        m_roomMessages = std::move(other.m_roomMessages);
    }
    return *this;
}

ClientAPI::Room::Room(const std::string& name, uint64_t creatorId, uint64_t roomId, int maxSize, const std::vector<Client>& clients)
    : m_name(name),
      m_creatorId(creatorId),
      m_roomId(roomId),
      m_maxSize(maxSize),
      m_clients(clients)
{
}

// ClientAPI::Room::Client
ClientAPI::Room::Client::Client(uint64_t id, const std::string& name)
    : m_id(id),
      m_name(name)
{
}

ClientAPI::Room::Client::Client(const Client& other)
    : m_id(other.m_id),
      m_name(other.m_name)
{
}

ClientAPI::Room::Client::Client(Client&& other)
    : m_id(std::move(other.m_id)),
      m_name(std::move(other.m_name))
{
}

ClientAPI::Room::Client& ClientAPI::Room::Client::operator=(const Client& other)
{
    if (this != &other)
    {
        m_id = other.m_id;
        m_name = other.m_name;
    }
    return *this;
}

ClientAPI::Room::Client& ClientAPI::Room::Client::operator=(Client&& other)
{
    if (this != &other)
    {
        m_id = std::move(other.m_id);
        m_name = std::move(other.m_name);
    }
    return *this;
}

///
/// ClientAPI
///

ClientAPI::ClientAPI(ServerData data)
    : m_data(data)
{
}

ClientAPI::ClientAPI(ClientAPI&& other)
    : m_data(std::move(other.m_data)),
      m_clientId(std::move(other.m_clientId)),
      m_currentMessage(std::move(other.m_currentMessage)),
      m_defaultRoom(std::move(other.m_defaultRoom)),
      m_currentlyActiveRooms(std::move(other.m_currentlyActiveRooms))
{
}

ClientAPI& ClientAPI::operator=(ClientAPI&& other)
{
    if (this != &other)
    {
        m_data = std::move(other.m_data);
        m_clientId = std::move(other.m_clientId);
        m_currentMessage = std::move(other.m_currentMessage);
        m_defaultRoom = std::move(other.m_defaultRoom);
        m_currentlyActiveRooms = std::move(other.m_currentlyActiveRooms);
    }
    return *this;
}

ClientAPI::ClientAPI(const ClientAPI& other)
    : m_data(other.m_data),
      m_clientId(other.m_clientId),
      m_currentMessage(other.m_currentMessage),
      m_defaultRoom(other.m_defaultRoom),
      m_currentlyActiveRooms(other.m_currentlyActiveRooms)
{
}

ClientAPI& ClientAPI::operator=(const ClientAPI& other)
{
    if (this != &other)
    {
        m_data = other.m_data;
        m_clientId = other.m_clientId;
        m_currentMessage = other.m_currentMessage;
        m_defaultRoom = other.m_defaultRoom;
        m_currentlyActiveRooms = other.m_currentlyActiveRooms;
    }
    return *this;
}

bool ClientAPI::IsInetUDPReady()
{
    return m_clientId &&
           !m_data.getInetUDPServerAddress().empty() &&
           m_data.getInetUDPServerPort() != 0xFFFF;
}

bool ClientAPI::isInetTCPReady()
{
    return m_clientId &&
           !getInetTCPServerAddress().empty() &&
           getInetTCPPortNumber() != 0xFFFF;
}

bool ClientAPI::isBoostTCPReady()
{
    return m_clientId &&
           !getBoostTCPServerAddress().empty() &&
           getBoostTCPServerPortNumber() != 0xFFFF;
}

bool ClientAPI::isBoostUDPReady()
{
    return m_clientId &&
           !getBoostUDPServerAddress().empty() &&
           getBoostUDPServerPortNumber() != 0xFFFF;
}

bool ClientAPI::isUnixDgramReady()
{
    return m_clientId &&
           !m_data.getUnixDgramServerPath().empty();
}

bool ClientAPI::isUnixStreamReady()
{
    return m_clientId != 0 &&
           !getUnixStreamPath().empty();
}

void ClientAPI::waitUntilInetUDPReady()
{
    while (!IsInetUDPReady())
    {
    }
}

void ClientAPI::waitUntilInetTCPReady()
{
    while (!isInetTCPReady())
    {
    }
}

void ClientAPI::waitUntilBoostTCPReady()
{
    while (!isBoostTCPReady())
    {
    }
}

void ClientAPI::waitUntilBoostUDPReady()
{
    while (!isBoostUDPReady())
    {
    }
}

void ClientAPI::waitUntilUnixDgramReady()
{
    while (!isUnixDgramReady())
    {
    }
}

void ClientAPI::waitUntilUnixStreamReady()
{
    while (!isUnixStreamReady())
    {
    }
}

bool ClientAPI::clientInRoom()
{
    auto rooms = getActiveRooms();

    for (auto r = rooms.begin(); r != rooms.end(); r++)
    {
        auto clients = r->getClients();
        for (auto c = clients.begin(); c != clients.end(); c++)
        {
            if (c->getId() == m_clientId)
            {
                return true;
            }
        }
    }
    return false;
}

uint64_t ClientAPI::clientRoomId()
{
    auto rooms = getActiveRooms();

    for (auto r = rooms.begin(); r != rooms.end(); r++)
    {
        auto clients = r->getClients();
        for (auto c = clients.begin(); c != clients.end(); c++)
        {
            if (c->getId() == m_clientId)
            {
                return r->getRoomId();
            }
        }
    }

    return 0;
}

bool ClientAPI::parse(boost::json::object json)
{
    // Parsing message.
    if (!json.contains("nexilis_status"))
    {
        Log::info("Running code without nexilis status");
        Json::print(json);
        return false;
    }

    if (json.contains("type"))
    {
        // This should be enumerated.
        if (json["type"] == "roomData")
        {
            if (json.find("rooms") != json.end())
            {
                auto rooms = json.at("rooms").as_array();
                std::vector<Room> newRooms;
                for (const auto& room : rooms)
                {
                    std::string name = room.at("name").as_string().c_str();
                    int maxSize = static_cast<int>(room.at("maxSize").as_int64());

                    uint64_t creatorId;
                    if (room.at("creatorId").if_uint64())
                    {
                        creatorId = room.at("creatorId").as_uint64();
                    }
                    else if (room.at("creatorId").if_int64())
                    {
                        creatorId = static_cast<uint64_t>(room.at("creatorId").as_int64());
                    }
                    else
                    {
                        creatorId = 0;
                    }

                    uint64_t id;
                    if (room.at("id").if_uint64())
                    {
                        id = room.at("id").as_uint64();
                    }
                    else if (room.at("id").if_int64())
                    {
                        id = static_cast<uint64_t>(room.at("id").as_int64());
                    }
                    else
                    {
                        id = 0;
                    }

                    std::vector<ClientAPI::Room::Client> roomClients;

                    if (room.as_object().find("clients") != room.as_object().end())
                    {
                        auto clients = room.at("clients").as_array();

                        for (const auto& client : clients)
                        {
                            uint64_t id;
                            if (client.at("id").if_uint64())
                            {
                                id = client.at("id").as_uint64();
                            }
                            else if (client.at("id").if_int64())
                            {
                                id = static_cast<uint64_t>(client.at("id").as_int64());
                            }
                            else
                            {
                                id = 0;
                            }

                            std::string clientName;
                            if (client.at("name").if_string())
                            {
                                clientName = client.at("name").as_string();
                            }
                            else
                            {
                                clientName = "NO NAME!";
                            }

                            ClientAPI::Room::Client newClient(id, clientName);
                            roomClients.emplace_back(std::move(newClient));
                        }
                    }

                    Log::info("ROOMCLIENTS AMOUNT: ", roomClients.size());

                    newRooms.emplace_back(Room(name, creatorId, id, maxSize, roomClients));
                }
                m_currentlyActiveRooms = newRooms;
                return true;
            }
            else
            {
                Log::warning("No rooms!");
                return false;
            }
        }
        else if (json["type"] == "set_client_id")
        {
            if (json["set_client_id"].if_uint64())
            {
                uint64_t id = json["set_client_id"].as_uint64();
                setClientId(id);
                Packet::_initialize(id);
                return true;
            }
            // boost::json::value is so bad.
            else if (json["set_client_id"].if_int64())
            {
                int64_t id = json["set_client_id"].as_int64();
                uint64_t u_id = id;

                assert(sizeof(id) == sizeof(u_id));
                assert(static_cast<uint64_t>(id) == u_id);

                setClientId(id);
                Packet::_initialize(id);
                return true;
            }
            else
            {
                Log::error("The value of set_client_id is not convertible to as_uint64");
                return false;
            }
        }

        else if (json["type"] == "room_message")
        {
            uint64_t id;
            if (json.at("id").if_uint64())
            {
                id = json.at("id").as_uint64();
            }
            else if (json.at("id").if_int64())
            {
                id = static_cast<uint64_t>(json.at("id").as_int64());
            }
            else
            {
                id = 0;
            }

            uint64_t roomId;
            if (json.at("roomId").if_uint64())
            {
                roomId = json.at("roomId").as_uint64();
            }
            else if (json.at("roomId").if_int64())
            {
                roomId = static_cast<uint64_t>(json.at("roomId").as_int64());
            }
            else
            {
                roomId = 0;
            }

            std::string message = json.at("message").as_string().c_str();

            assert(!message.empty());
            assert(id != 0);
            assert(roomId != 0);


            for (auto room = m_currentlyActiveRooms.begin(); room != m_currentlyActiveRooms.end(); room++)
            {
                if (room->getRoomId() == roomId)
                {
                    Room::Communication newMessage(message, id);
                    room->addMessage(std::move(newMessage));
                    return true;
                }
            }
            Log::info("Client not in the room it's targetting!");
            return false;
        }

        else
        {
            Log::info("UNDEFINED TYPE ", json["type"]);
            return false;
        }
    }
    // TODO continue parsing.
    return true;
}

bool ClientAPI::readMessage(std::vector<uint8_t> message)
{
    boost::json::object json;
    try
    {
        json = Json::convertToJSON(message);
    }
    catch (...)
    {
        Log::error("Cannot convert message to json");
        Log::debug("Trying to debug json");

        try
        {
            Json::print(json);
        }
        catch (...)
        {
            Log::debug("Something wrong with message: ", boost::json::serialize(json));
        }
        return false;
    }


    if (parse(json))
    {
        m_currentMessage = json;
        return true;
    }
    else
    {
        Log::info("Received message that is not read by the server");
        return false;
    }
}

ClientAPI::Room& ClientAPI::roomWhereClientIs(uint64_t clientId)
{
    for (auto& room : m_currentlyActiveRooms)
    {
        for (auto& client : room.getClients())
        {
            if (client.getId() == clientId)
            {
                return room;
            }
        }
    }
    return m_defaultRoom;
}

} // namespace nexilis
