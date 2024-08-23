#include <nexilis/room_storage.hh>
#include <nexilis/client_api.hh>
#include <nexilis/common/util.hh>
#include <nexilis/json.hh>
#include <nexilis/packet.hh>
#include <nexilis/log.hh>

#include <thread>

namespace nexilis
{

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

// ClientAPI::Room::Communication
ClientAPI::Room::Communication::Communication(const std::string& payload, ClientAPI::Room::Client* client) :
    m_payload(payload),
    m_client(client),
    m_id(Util::getRandomUint64())
{
}

ClientAPI::Room::Communication::Communication(const Communication& other) :
    m_payload(other.m_payload),
    m_client(other.m_client),
    m_id(other.m_id)
{
}

ClientAPI::Room::Communication& ClientAPI::Room::Communication::operator=(const Communication& other)
{
    if (this != &other)
    {
        m_payload = other.m_payload;
        m_client = other.m_client;
        m_id = other.m_id;
    }
    return *this;
}

ClientAPI::Room::Communication::Communication(Communication&& other) :
    m_payload(std::move(other.m_payload)),
    m_client(std::move(other.m_client)),
    m_id(std::move(other.m_id))

{
}

ClientAPI::Room::Communication& ClientAPI::Room::Communication::operator=(Communication&& other)
{
    if (this != &other)
    {
        m_payload = std::move(other.m_payload);
        m_client = std::move(other.m_client);
        m_id = std::move(other.m_id);
    }
    return *this;
}

bool operator==(const ClientAPI::Room::Communication& lhs, const ClientAPI::Room::Communication& rhs)
{
    return lhs.getPayload() == rhs.getPayload() &&
           lhs.getClient() == rhs.getClient() &&
           lhs.getId() == rhs.getId();
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

bool operator==(const ClientAPI::Room::Client& lhs, const ClientAPI::Room::Client& rhs)
{
    return lhs.getId() == rhs.getId() &&
        lhs.getUsername() == rhs.getUsername();
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

bool operator==(const ClientAPI::Room& lhs, const ClientAPI::Room& rhs)
{
    return lhs.getName() == rhs.getName() &&
       lhs.getCreatorId() == rhs.getCreatorId() &&
       lhs.getRoomId() == rhs.getRoomId() &&
       lhs.getMaxSize() == rhs.getMaxSize() &&
       lhs.getClients() == rhs.getClients() &&
       lhs.getMessages() == rhs.getMessages();
}

bool ClientAPI::Room::containsCommunication(const Room::Communication& communication)
{
    return std::find(m_roomMessages.begin(), m_roomMessages.end(), communication) != m_roomMessages.end();
}

bool ClientAPI::Room::containsCommunication(uint64_t communicationId)
{
    auto idComparator = [communicationId](const Communication& item)
    {
        return item.getId() == communicationId;
    };

    return std::find_if(m_roomMessages.begin(), m_roomMessages.end(), idComparator) != m_roomMessages.end();
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
      m_currentlyActiveRooms(std::move(other.m_currentlyActiveRooms)),
      m_messageIds(std::move(other.m_messageIds)),
      m_callbacks(std::move(other.m_callbacks))
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
        m_messageIds = std::move(other.m_messageIds);
        m_callbacks = std::move(other.m_callbacks);
    }
    return *this;
}

ClientAPI::ClientAPI(const ClientAPI& other)
    : m_data(other.m_data),
      m_clientId(other.m_clientId),
      m_currentMessage(other.m_currentMessage),
      m_defaultRoom(other.m_defaultRoom),
      m_currentlyActiveRooms(other.m_currentlyActiveRooms),
      m_messageIds(other.m_messageIds),
      m_callbacks(other.m_callbacks)
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
        m_messageIds = other.m_messageIds;
        m_callbacks = other.m_callbacks;
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

uint64_t ClientAPI::getNewMessageId()
{
    uint64_t newId = Util::getRandomUint64();

    if (std::find(m_messageIds.begin(), m_messageIds.end(), newId) != m_messageIds.end())
    {
        return getNewMessageId();
    }
    else
    {
        m_messageIds.emplace_back(newId);
        return newId;
    }
}

void ClientAPI::addCallback(const std::pair<uint64_t, const std::function<void()>&>& callback)
{
    Log::warning("Added callback with id: ", callback.first);
    m_callbacks.emplace_back(callback);
}

ClientAPI::ReadResult ClientAPI::readCommand(boost::json::object json)
{
    // Parsing message.
    if (!json.contains("nexilis_status"))
    {
        Log::info("Running code without nexilis status");
        return ReadResult::missing_nexilis_status;
    }

    if (json.contains("command") && json.contains("type"))
    {
        auto command = json["command"];
        auto type = json["type"];

        if (command == "set")
        {
            if (type == "username")
            {
                std::string a = json.at("set_username").as_string().data();

                // Set own m_data.
                m_data.setUserName(a);
                return ReadResult::success;

                /*
                for (const auto& rooms : m_currentlyActiveRooms)
                {
                    for (auto& client : rooms.getClients())
                    {
                        if (client.getId() == m_clientId)
                        {
                            client.setUsername(a);
                            return ReadResult::success;
                        }
                    }
                }
                return ReadResult::not_found;
                */
            }
            else
            {
                return ReadResult::error;
            }
        }

        else if (command == "get")
        {
            if (type == "set_client_id")
            {
                if (json["set_client_id"].if_uint64())
                {
                    uint64_t id = json["set_client_id"].as_uint64();
                    setClientId(id);
                    Packet::_initialize(*this);
                    return ReadResult::success;
                }
                // boost::json::value is so bad.
                else if (json["set_client_id"].if_int64())
                {
                    int64_t id = json["set_client_id"].as_int64();
                    uint64_t u_id = id;

                    assert(sizeof(id) == sizeof(u_id));
                    assert(static_cast<uint64_t>(id) == u_id);

                    setClientId(id);
                    Packet::_initialize(*this);
                    return ReadResult::success;
                }
                else
                {
                    Log::error("The value of set_client_id is not convertible to as_uint64");
                    return ReadResult::error;
                }
            }
            else
            {
                Log::error("Unused path");
                return ReadResult::not_found;
            }
        }

        else if (command == "info")
        {
            if (type == "room_data")
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
                        newRooms.emplace_back(Room(name, creatorId, id, maxSize, roomClients));
                    }
                    m_currentlyActiveRooms = newRooms;
                    return ReadResult::success;
                }
            }
            else if (type == "client_data")
            {
                Log::error("Not implemented");
                return ReadResult::not_implemented;
            }
            else if (type == "server_data")
            {
                Log::error("Not implemented");
                return ReadResult::not_implemented;
            }
            else
            {
                Log::error("Wrong type!");
                return ReadResult::not_implemented;
            }
        }

        else if (command == "communicate")
        {
            if (type == "room_message")
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

                for (auto&& room = m_currentlyActiveRooms.begin(); room != m_currentlyActiveRooms.end(); room++)
                {
                    Room::Client* sender = nullptr;
                    for (auto& client : room->getClients())
                    {
                        if (client.getId() == id)
                        {
                            sender = &client;
                        }
                    }

                    if (room->getRoomId() == roomId)
                    {
                        Room::Communication newMessage(message, sender);
                        room->addMessage(std::move(newMessage));

                        assert(room->containsCommunication(newMessage.getId()));
                        Log::info("Added new message in room: ", roomId);
                        return ReadResult::success;
                    }
                }
                Log::info("Client not in the room it's targetting!");
                return ReadResult::failure;
            }
            else if (type == "broadcast")
            {
                Log::error("Not implemented");
                return ReadResult::not_implemented;
            }
            else if (type == "multicast")
            {
                Log::error("Not implemented");
                return ReadResult::not_implemented;
            }
        }
        else if (command == "room")
        {
            if (type == "join")
            {
                // This is a valid command, but there is nothing to do.
                return ReadResult::success;
            }
        }
        else
        {
            Log::error("UNDEFINED TYPE");
            return ReadResult::not_found;
        }
    }
    return ReadResult::clean;
}

ClientAPI::ReadResult ClientAPI::readMessage(const std::vector<uint8_t>& message)
{
    boost::json::object json;
    try
    {
        json = Json::convertToJSON(message);
    }
    catch (...)
    {
        Util::debugUint8Vector(message);
        return ReadResult::error;
    }

    auto result = readCommand(json);

    if (json.find("callback") != json.end() && json["callback"] != 0)
    {
        readCallback(json["callback"]);
    }

    if (result == ReadResult::success)
    {
        m_currentMessage = json;
    }
    else
    {
        Log::error("Received message that is not read by the server");
        std::string stringMessage = Util::convertToString(message);
        Log::error("Data: ", stringMessage);
    }
    return result;
}

void ClientAPI::readCallback(boost::json::value callback)
{
    uint64_t cb;
    if (callback.if_uint64())
    {
        cb = callback.as_uint64();
    }
    else if (callback.if_int64())
    {
        cb = static_cast<uint64_t>(callback.as_int64());
    }
    else
    {
        cb = 0;
    }

    Log::debug("Callback id: ", cb);

    // Calling callback.
    for (auto it = m_callbacks.begin(); it != m_callbacks.end(); ++it)
    {
        if (it->first == cb)
        {
            it->second();
            it = m_callbacks.erase(it);
            break;
        }
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

std::function<void()> ClientAPI::waitUntilRoomsCreated(std::promise<void>& promise)
{
    return [&promise, this]()
    {
        // Because all the rooms are created as once, we basically check if the rooms exist.
        while (getActiveRooms().size() < 1)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        promise.set_value();
    };
}

} // namespace nexilis
