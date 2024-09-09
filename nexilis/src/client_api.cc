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
      m_applicationType(std::move(other.m_applicationType)),
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
      m_applicationType(other.m_applicationType),
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
        m_applicationType = std::move(other.m_applicationType);
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
        m_applicationType = other.m_applicationType;
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

// ClientAPI::ClientSession
ClientAPI::ClientSession::ClientSession(uint64_t id, ClientAPI* clientAPI)
    : BaseClient(id),
      m_clientAPI(clientAPI)
{
}

ClientAPI::ClientSession::ClientSession(ClientSession&& other)
    : BaseClient(std::move(other)),
      m_clientAPI(std::move(other.m_clientAPI))
{
}

ClientAPI::ClientSession& ClientAPI::ClientSession::operator=(ClientSession&& other)
{
    if (this != &other)
    {
        BaseClient::operator=(std::move(other));
    }
    return *this;
}

bool operator==(const ClientAPI::ClientSession& lhs, const ClientAPI::ClientSession& rhs)
{
    return lhs.getId() == rhs.getId() &&
        lhs.getUsername() == rhs.getUsername();
}

// ClientAPI::Room::Communication
ClientAPI::Room::Communication::Communication(const std::string& payload, ClientAPI::ClientSession* client) :
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


// ClientAPI::Room
ClientAPI::Room::Room(Room&& other) :
    m_name(std::move(other.m_name)),
    m_creatorId(std::move(other.m_creatorId)),
    m_roomId(std::move(other.m_roomId)),
    m_maxSize(std::move(other.m_maxSize)),
    m_clients(std::move(other.m_clients)),
    m_roomMessages(std::move(other.m_roomMessages))
{
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

ClientAPI::Room::Room(const std::string& name, uint64_t creatorId, uint64_t roomId, int maxSize, std::vector<ClientSession>&& clients)
    : m_name(name),
      m_creatorId(creatorId),
      m_roomId(roomId),
      m_maxSize(maxSize),
      m_clients(std::move(clients))
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
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void ClientAPI::waitUntilInetTCPReady()
{
    while (!isInetTCPReady())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void ClientAPI::waitUntilBoostTCPReady()
{
    while (!isBoostTCPReady())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void ClientAPI::waitUntilBoostUDPReady()
{
    while (!isBoostUDPReady())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void ClientAPI::waitUntilUnixDgramReady()
{
    while (!isUnixDgramReady())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void ClientAPI::waitUntilUnixStreamReady()
{
    while (!isUnixStreamReady())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

bool ClientAPI::clientInRoom()
{
    auto& rooms = getActiveRooms();

    for (auto r = rooms.begin(); r != rooms.end(); r++)
    {
        auto& clients = r->getClients();
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
    auto& rooms = getActiveRooms();

    for (auto r = rooms.begin(); r != rooms.end(); r++)
    {
        auto& clients = r->getClients();
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

void ClientAPI::addCallback(const std::pair<uint64_t, const std::function<void()>>& callback)
{
    m_callbacks.emplace_back(callback);
}

ClientAPI::ReadResult ClientAPI::readCommand(boost::json::object json)
{
    if (json.contains("command") && json.contains("type"))
    {
        auto command = json["command"];
        auto type = json["type"];

        if (command == "set")
        {
            if (type == "username")
            {
                std::string username = readString(json, "username");

                // Set own m_data.
                m_data.setUserName(username);
                return ReadResult::success;

                for (auto&& rooms : m_currentlyActiveRooms)
                {
                    for (auto&& client : rooms.getClients())
                    {
                        if (client.getId() == m_clientId)
                        {
                            client.setUsername(username);
                            return ReadResult::success;
                        }
                    }
                }
                return ReadResult::not_found;
            }
            else
            {
                return ReadResult::error;
            }
        }

        else if (command == "get")
        {
            if (type == "client_id")
            {
                uint64_t clientId = readUint64(json, "client_id");
                setClientId(clientId);
                Packet::_initialize(*this);
                return ReadResult::success;
            }
            else
            {
                Log::error("Unused path");
                return ReadResult::not_found;
            }
        }

        else if (command == "room")
        {
            uint64_t roomId = readUint64(json, "roomId");
            uint64_t clientId = readUint64(json, "clientId");

            if (type == "join")
            {
                for (auto& room : m_currentlyActiveRooms)
                {
                    if (room.getRoomId() == roomId)
                    {
                        ClientSession session(clientId, this);
                        room.addClient(std::move(session));
                        return ReadResult::success;
                    }
                }
                return ReadResult::error;
            }
            else if (type == "leave")
            {
                for (auto& room : m_currentlyActiveRooms)
                {
                    if (room.getRoomId() == roomId)
                    {
                        room.removeClient(clientId);
                        return ReadResult::success;
                    }
                }
                return ReadResult::failure;
            }
            else if (type == "create")
            {
                auto room = ClientAPI::Room();
            }
            else
            {
                Log::error("No such room command!");
                return ReadResult::failure;
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
                        std::string name = readString(room, "name");
                        int maxSize = static_cast<int>(room.at("maxSize").as_int64());

                        uint64_t creatorId = readUint64(room, "creatorId");
                        uint64_t id = readUint64(room, "id");

                        std::vector<ClientAPI::ClientSession> roomClients;

                        if (room.as_object().find("clients") != room.as_object().end())
                        {
                            auto clients = room.at("clients").as_array();
                            Log::info("Clients size: ", clients.size());

                            for (const auto& client : clients)
                            {
                                uint64_t id = readUint64(client, "id");
                                std::string username = readString(client, "name");

                                float object2DX = readFloat(client, "2dPosX");
                                float object2DY = readFloat(client, "2dPosY");
                                float dimension2DX = readFloat(client, "2dDimensionX");
                                float dimension2DY = readFloat(client, "2dDimensionY");

                                ClientAPI::ClientSession newClient(id, this);
                                Log::info("Position set in room x: ", object2DX, " y: ", object2DY);
                                newClient.getObject2D().setPosition(object2DX, object2DY);
                                newClient.getObject2D().setDimensions(dimension2DX, dimension2DY);
                                newClient.setUsername(username);
                                roomClients.emplace_back(std::move(newClient));
                            }
                        }
                        newRooms.emplace_back(Room(name, creatorId, id, maxSize, std::move(roomClients)));
                    }
                    m_currentlyActiveRooms = std::move(newRooms);
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
                uint64_t id = readUint64(json, "id");
                uint64_t roomId = readUint64(json, "roomId");
                std::string message = readString(json, "message");

                for (auto&& room = m_currentlyActiveRooms.begin(); room != m_currentlyActiveRooms.end(); room++)
                {
                    ClientSession* sender = nullptr;
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
        else if (command == "position")
        {
            if (type == "vector2")
            {
                uint64_t id = readUint64(json, "id");
                float vectorX = readFloat(json, "positionX");
                float vectorY = readFloat(json, "positionY");

                for (auto&& room = m_currentlyActiveRooms.begin(); room != m_currentlyActiveRooms.end(); room++)
                {
                    for (auto& client : room->getClients())
                    {
                        if (client.getId() == id)
                        {
                            if (overlappingAllowed())
                            {
                                client.getObject2D().setPosition(vectorX, vectorY);
                            }
                            else
                            {
                                for (auto& otherClient : room->getClients())
                                {
                                    if (otherClient.getId() != id)
                                    {
                                        auto dimensions = client.getObject2D().getDimensions();
                                        auto otherPosition = otherClient.getObject2D().getPosition();
                                        auto otherdimensions = otherClient.getObject2D().getDimensions();

                                        if (
                                            vectorX - dimensions.x / 2 < otherPosition.x + otherdimensions.x / 2 &&
                                            vectorX + dimensions.x / 2 > otherPosition.x - otherdimensions.x / 2 &&
                                            vectorY - dimensions.y / 2 < otherPosition.y + otherdimensions.y / 2 &&
                                            vectorY + dimensions.y / 2 > otherPosition.y - otherdimensions.y / 2
                                            )
                                        {
                                            return ReadResult::failure;
                                        }
                                    }
                                }
                                client.getObject2D().setPosition(vectorX, vectorY);
                           }
                        }
                    }
                }

                return ReadResult::success;
            }
        }
        else if (command == "dimensions")
        {
            if (type == "vector2")
            {
                uint64_t id = readUint64(json, "id");
                float vectorX = readFloat(json, "x");
                float vectorY = readFloat(json, "y");

                for (auto&& room = m_currentlyActiveRooms.begin(); room != m_currentlyActiveRooms.end(); room++)
                {
                    for (auto& client : room->getClients())
                    {
                        if (client.getId() == id)
                        {
                            client.getObject2D().setDimensions(vectorX, vectorY);
                            return ReadResult::success;
                        }
                    }
                }
                return ReadResult::failure;
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
    else if (result == ReadResult::failure)
    {
        Log::warning("Failure in command");
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

std::string ClientAPI::readString(const boost::json::value& context, const std::string& key)
{
    std::string item;
    bool readGood = true;
    if (context.at(key).if_string())
    {
        item = context.at(key).as_string();
    }
    else
    {
        readGood = false;
    }
    assert(readGood);
    return item;
}

uint64_t ClientAPI::readUint64(const boost::json::value& context, const std::string& key)
{
    uint64_t item;
    bool readGood = true;
    if (context.at(key).if_uint64())
    {
        item = context.at(key).as_uint64();
    }
    else if (context.at(key).if_int64())
    {
        item = static_cast<uint64_t>(context.at(key).as_int64());
    }
    else
    {
        readGood = false;
    }
    Log::info("Read Uint64 item:  ", item, " key: ", key);
    assert(readGood);
    return item;
}

float ClientAPI::readFloat(const boost::json::value& context, const std::string& key)
{
    float item;
    bool readGood = true;
    if (context.at(key).if_double())
    {
        item = context.at(key).as_double();
    }
    else
    {
        readGood = false;
    }
    assert(readGood);
    return item;
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
