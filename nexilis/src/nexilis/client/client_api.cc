#include <nexilis/client/client_api.hh>
#include <nexilis/client/client_session.hh>
#include <nexilis/client/packet.hh>
#include <nexilis/command_type.hh>
#include <nexilis/json.hh>
#include <nexilis/logger/log.hh>
#include <nexilis/util.hh>

#include <string>
#include <thread>

namespace nexilis::client
{

ClientAPI::ClientAPI(ServerData data)
    : NxClass("ClientAPI"),
      m_data(data),
      m_roomsMutex(std::make_unique<std::mutex>())
{
}

ClientAPI::ClientAPI(ClientAPI&& other)
    : NxClass(std::move(other)),
      m_data(std::move(other.m_data)),
      m_clientId(std::move(other.m_clientId)),
      m_currentlyActiveRooms(std::move(other.m_currentlyActiveRooms)),
      m_messageIds(std::move(other.m_messageIds)),
      m_callbacks(std::move(other.m_callbacks)),
      m_roomsMutex(std::move(other.m_roomsMutex))
{
}

ClientAPI& ClientAPI::operator=(ClientAPI&& other)
{
    if (this != &other)
    {
        m_data = std::move(other.m_data);
        m_clientId = std::move(other.m_clientId);
        m_currentlyActiveRooms = std::move(other.m_currentlyActiveRooms);
        m_messageIds = std::move(other.m_messageIds);
        m_callbacks = std::move(other.m_callbacks);
        m_roomsMutex = std::move(other.m_roomsMutex);

        NxClass::operator=(std::move(other));
    }
    return *this;
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
                return r->getId();
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
    if (!json.contains("command") || !json.contains("type"))
    {
        return ReadResult::error;
    }

    auto command = commandTypeFromString(json["command"].as_string().c_str());
    auto type = json["type"];
    Log::debug(header(), "Received type: ", type, " message");

    switch (command)
    {
        case CommandType::setting:
        {
            if (type == "username")
            {
                std::string username = createString(json, "username");
                std::lock_guard<std::mutex> lock(*m_roomsMutex);

                // The clients that we are avare of.
                auto* client = getClientFromRoom(m_clientId);
                if (client)
                {
                    client->setUsername(username);
                }
                return ReadResult::success;
            }
            // FIXME this is so bad.
            else if (type == "port")
            {
                std::lock_guard<std::mutex> lock(*m_roomsMutex);
                uint16_t port = createUint64(json, "port");
                m_data.setBoostTCPPortNumber(port);
                return ReadResult::success;
            }
            else
            {
                return ReadResult::not_found;
            }
        }
        case CommandType::getting:
        {
            if (type == "client_id")
            {
                std::lock_guard<std::mutex> lock(*m_roomsMutex);

                uint64_t clientId = createUint64(json, "client_id");
                setClientId(clientId);
                Packet::_initialize(*this);
                m_isInitialized = true;
                return ReadResult::success;
            }
            else
            {
                Log::error("Unused path");
                return ReadResult::not_found;
            }
        }

        case CommandType::room:
        {
            uint64_t roomId = createUint64(json, "roomId");
            uint64_t clientId = createUint64(json, "clientId");
            std::string roomAction = createString(json, "action");

            // Skipping messages where the client does not have to be in a room.
            if (type != "management")
            {
                if (!clientInRoom())
                {
                    Log::error("Client is missing room for type: ", type, " roomaction: ", roomAction);
                    return ReadResult::client_missing_room;
                }
            }

            if (type == "management")
            {
                if (roomAction == "join")
                {
                    std::lock_guard<std::mutex> lock(*m_roomsMutex);
                    for (auto& room : m_currentlyActiveRooms)
                    {
                        if (room.getId() == roomId)
                        {
                            ClientSession session(clientId, this);
                            room.addClient(std::move(session));
                            return ReadResult::success;
                        }
                    }
                    return ReadResult::error;
                }
                else if (roomAction == "leave")
                {
                    std::lock_guard<std::mutex> lock(*m_roomsMutex);
                    for (auto& room : m_currentlyActiveRooms)
                    {
                        if (room.getId() == roomId)
                        {
                            room.removeClient(clientId);
                            return ReadResult::success;
                        }
                    }
                    return ReadResult::error;
                }
                else if (roomAction == "create")
                {
                    // TODO
                    return ReadResult::not_implemented;
                }
                return ReadResult::error;
            }
            else if (type == "player2D")
            {
                if (roomAction == "position" || roomAction == "movement")
                {
                    float vectorX = createFloat(json, "x");
                    float vectorY = createFloat(json, "y");
                    std::lock_guard<std::mutex> lock(*m_roomsMutex);

                    for (auto&& room = m_currentlyActiveRooms.begin(); room != m_currentlyActiveRooms.end(); room++)
                    {
                        for (auto& client : room->getClients())
                        {
                            if (client.getId() == clientId)
                            {
                                if (overlappingAllowed2D())
                                {
                                    client.getObject2D().setPosition({vectorX, vectorY});
                                    return ReadResult::success;
                                }
                                else
                                {
                                    for (auto& otherClient : room->getClients())
                                    {
                                        if (otherClient.getId() != clientId)
                                        {
                                            auto dimensions = client.getObject2D().getDimensions();
                                            auto otherPosition = otherClient.getObject2D().getPosition();
                                            auto otherdimensions = otherClient.getObject2D().getDimensions();

                                            if (
                                                    vectorX - dimensions.x / 2 < otherPosition.x + otherdimensions.x / 2 &&
                                                    vectorX + dimensions.x / 2 > otherPosition.x - otherdimensions.x / 2 &&
                                                    vectorY - dimensions.y / 2 < otherPosition.y + otherdimensions.y / 2 &&
                                                    vectorY + dimensions.y / 2 > otherPosition.y - otherdimensions.y / 2)
                                            {
                                                return ReadResult::failure;
                                            }
                                        }
                                    }
                                    client.getObject2D().setPosition({vectorX, vectorY});
                                    return ReadResult::success;
                                }
                            }
                        }
                        return ReadResult::clean;
                    }
                }
                else if (roomAction == "dimensions")
                {
                    float vectorX = createFloat(json, "x");
                    float vectorY = createFloat(json, "y");
                    std::lock_guard<std::mutex> lock(*m_roomsMutex);

                    for (auto&& room = m_currentlyActiveRooms.begin(); room != m_currentlyActiveRooms.end(); room++)
                    {
                        for (auto& client : room->getClients())
                        {
                            if (client.getId() == clientId)
                            {
                                client.getObject2D().setDimensions({vectorX, vectorY});
                                return ReadResult::success;
                            }
                        }
                    }
                    return ReadResult::failure;
                }
                else
                {
                    return ReadResult::not_found;
                }
            }
            else if (type == "player3D")
            {
                if (roomAction == "position")
                {
                    float x = createFloat(json, "x");
                    float y = createFloat(json, "y");
                    float z = createFloat(json, "z");
                    auto client = getClientFromRoom(clientId);
                    std::lock_guard<std::mutex> lock(*m_roomsMutex);
                    if (client)
                    {
                        client->getObject3D().setPosition(Vector3(x, y, z));
                        return ReadResult::success;
                    }
                    else
                    {
                        return ReadResult::client_missing_room;
                    }
                }
                else if (roomAction == "dimensions")
                {
                    float x = createFloat(json, "x");
                    float y = createFloat(json, "y");
                    float z = createFloat(json, "z");
                    auto client = getClientFromRoom(clientId);
                    std::lock_guard<std::mutex> lock(*m_roomsMutex);
                    if (client)
                    {
                        client->getObject3D().setDimensions(Vector3(x, y, z));
                        return ReadResult::success;
                    }
                    else
                    {
                        return ReadResult::client_missing_room;
                    }
                }
                else if (roomAction == "movement")
                {
                    float vector_x = createFloat(json, "x");
                    float vector_y = createFloat(json, "y");
                    float vector_z = createFloat(json, "z");
                    auto pos = Vector3f(vector_x, vector_y, vector_z);

                    std::lock_guard<std::mutex> lock(*m_roomsMutex);

                    for (auto&& room = m_currentlyActiveRooms.begin(); room != m_currentlyActiveRooms.end(); room++)
                    {
                        for (auto& client : room->getClients())
                        {
                            if (client.getId() == clientId)
                            {
                                client.getObject3D().setPosition(pos);
                                return ReadResult::success;
                            }
                        }
                    }
                    return ReadResult::clean;
                }
                else
                {
                    return ReadResult::not_found;
                }
            }
            else if (type == "object2D")
            {
                if (roomAction == "create")
                {
                    float positionX = createFloat(json, "positionX");
                    float positionY = createFloat(json, "positionY");
                    float dimensionX = createFloat(json, "dimensionX");
                    float dimensionY = createFloat(json, "dimensionY");
                    std::string filePath = createString(json, "filepath");
                    uint64_t id = createUint64(json, "id");
                    std::lock_guard<std::mutex> lock(*m_roomsMutex);

                    for (auto& room : m_currentlyActiveRooms)
                    {
                        if (room.getId() == roomId)
                        {
                            auto object = Object2D(id, {positionX, positionY}, {dimensionX, dimensionY});
                            object.setFilepath(filePath);
                            room.addObject(std::move(object));
                            return ReadResult::success;
                        }
                    }
                    return ReadResult::failure;
                }
                else if (roomAction == "destroy")
                {
                    uint64_t objectId = createUint64(json, "id");
                    std::lock_guard<std::mutex> lock(*m_roomsMutex);
                    for (auto& room : m_currentlyActiveRooms)
                    {
                        if (room.getId() == roomId)
                        {
                            room.deleteObject2D(objectId);
                            return ReadResult::success;
                        }
                    }
                    return ReadResult::failure;
                }
                else if (roomAction == "move")
                {
                    uint64_t objectId = createUint64(json, "objectId");
                    float newPositionX = createFloat(json, "x");
                    float newPositionY = createFloat(json, "y");
                    std::lock_guard<std::mutex> lock(*m_roomsMutex);

                    for (auto& room : m_currentlyActiveRooms)
                    {
                        if (room.getId() == roomId)
                        {
                            auto object = room.getObject2DById(objectId);
                            object->setPosition({newPositionX, newPositionY});
                            return ReadResult::success;
                        }
                    }
                    return ReadResult::failure;
                }
                else if (roomAction == "createMoving")
                {
                    std::string createMovingType = createString(json, "createMovingType");

                    if (createMovingType == "create")
                    {
                        float positionX = createFloat(json, "positionX");
                        float positionY = createFloat(json, "positionY");
                        float dimensionX = createFloat(json, "dimensionX");
                        float dimensionY = createFloat(json, "dimensionY");
                        std::string filepath = createString(json, "filepath");
                        uint64_t id = createUint64(json, "id");
                        std::lock_guard<std::mutex> lock(*m_roomsMutex);

                        for (auto& room : m_currentlyActiveRooms)
                        {
                            if (room.getId() == roomId)
                            {
                                auto object = Object2D(id, {positionX, positionY}, {dimensionX, dimensionY});
                                object.setFilepath(filepath);
                                room.addObject(std::move(object));
                                return ReadResult::success;
                            }
                        }
                        return ReadResult::failure;
                    }
                    else if (createMovingType == "update")
                    {
                        float positionX = createFloat(json, "x");
                        float positionY = createFloat(json, "y");
                        uint64_t objectId = createUint64(json, "objectId");
                        std::lock_guard<std::mutex> lock(*m_roomsMutex);

                        for (auto& room : m_currentlyActiveRooms)
                        {
                            if (room.getId() == roomId)
                            {
                                auto* object = room.getObject2DById(objectId);
                                if (!object)
                                {
                                    Log::error("Could not find object!");
                                    return ReadResult::error;
                                }
                                object->setPosition({positionX, positionY});
                                return ReadResult::success;
                            }
                        }
                        return ReadResult::failure;
                    }
                    else
                    {
                        Log::error("Undefined createmovingtype");
                        return ReadResult::failure;
                    }
                }
                else if (roomAction == "createMovingTest")
                {
                    std::string createMovingType = createString(json, "createMovingType");

                    if (createMovingType == "create")
                    {
                        float positionX = createFloat(json, "positionX");
                        float positionY = createFloat(json, "positionY");
                        float dimensionX = createFloat(json, "dimensionX");
                        float dimensionY = createFloat(json, "dimensionY");
                        std::string filepath = createString(json, "filepath");
                        uint64_t id = createUint64(json, "id");
                        std::lock_guard<std::mutex> lock(*m_roomsMutex);

                        for (auto& room : m_currentlyActiveRooms)
                        {
                            if (room.getId() == roomId)
                            {
                                auto object = Object2D(id, {positionX, positionY}, {dimensionX, dimensionY});
                                object.setFilepath(filepath);
                                room.addObject(std::move(object));
                                return ReadResult::success;
                            }
                        }
                        return ReadResult::failure;
                    }
                    else if (createMovingType == "update")
                    {
                        float positionX = createFloat(json, "x");
                        float positionY = createFloat(json, "y");
                        uint64_t objectId = createUint64(json, "objectId");
                        std::lock_guard<std::mutex> lock(*m_roomsMutex);

                        for (auto& room : m_currentlyActiveRooms)
                        {
                            if (room.getId() == roomId)
                            {
                                auto* object = room.getObject2DById(objectId);
                                if (!object)
                                {
                                    Log::error("Could not find object!");
                                    return ReadResult::error;
                                }
                                object->setPosition({positionX, positionY});
                                return ReadResult::success;
                            }
                        }
                        return ReadResult::failure;
                    }
                    else
                    {
                        Log::error("Undefined createmovingtype");
                        return ReadResult::failure;
                    }
                }
            }
            else if (type == "communication")
            {
                if (roomAction == "broadcast")
                {
                    uint64_t id = createUint64(json, "id");
                    std::string message = createString(json, "message");
                    std::lock_guard<std::mutex> lock(*m_roomsMutex);

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

                        if (room->getId() == roomId)
                        {
                            Room::Communication newMessage(message, sender);
                            auto message_id = newMessage.getId();
                            room->addMessage(std::move(newMessage));

                            assert(room->containsCommunication(message_id));
                            Log::info("Added new message in room: ", roomId);
                            return ReadResult::success;
                        }
                    }
                    Log::info("Client not in the room it's targetting!");
                    return ReadResult::failure;
                }
                else if (roomAction == "broadcast")
                {
                    Log::error("Not implemented");
                    return ReadResult::not_implemented;
                }
                else if (roomAction == "multicast")
                {
                    Log::error("Not implemented");
                    return ReadResult::not_implemented;
                }
                else
                {
                    Log::error("No such room command!");
                    return ReadResult::failure;
                }
            }
        }
        case CommandType::authentication:
        case CommandType::server_management:
        case CommandType::player_management:
            break;
        case CommandType::error:
            return ReadResult::error;

        case CommandType::info:
        {
            if (type == "room_data")
            {
                if (json.find("rooms") != json.end())
                {
                    auto rooms = json.at("rooms").as_array();
                    std::vector<Room> newRooms;
                    for (const auto& room : rooms)
                    {
                        std::string name = createString(room, "name");
                        uint64_t maxSize = createUint64(room, "maxSize");
                        uint64_t context = createUint64(room, "context");
                        uint64_t creatorId = createUint64(room, "creatorId");
                        uint64_t id = createUint64(room, "id");

                        std::vector<ClientSession> roomClients;

                        if (room.as_object().find("clients") != room.as_object().end())
                        {
                            auto clients = room.at("clients").as_array();
                            Log::info("Clients size: ", clients.size());

                            for (const auto& client : clients)
                            {
                                uint64_t client_id = createUint64(client, "id");
                                std::string username = createString(client, "name");

                                float object2DX = createFloat(client, "roomPositionX");
                                float object2DY = createFloat(client, "roomPositionY");
                                float dimension2DX = createFloat(client, "roomDimensionX");
                                float dimension2DY = createFloat(client, "roomDimensionY");

                                ClientSession newClient(client_id, this);
                                Log::info("Position set in room x: ", object2DX, " y: ", object2DY);
                                newClient.getObject2D().setPosition({object2DX, object2DY});
                                newClient.getObject2D().setDimensions({dimension2DX, dimension2DY});
                                newClient.setUsername(username);
                                roomClients.emplace_back(std::move(newClient));
                            }
                        }
                        auto roomData = RoomData(creatorId, name, id, static_cast<RoomData::Context>(context), maxSize);
                        newRooms.emplace_back(Room(roomData, std::move(roomClients)));
                    }
                    std::lock_guard<std::mutex> lock(*m_roomsMutex);
                    m_currentlyActiveRooms = std::move(newRooms);
                    FileLog::debug("Currently active rooms in ClientAPI: ", m_currentlyActiveRooms.size());
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
                return ReadResult::not_found;
            }
            return ReadResult::not_found;
        }
        case CommandType::undefined:
            return ReadResult::error;
        default:
            return ReadResult::error;
    }
    return ReadResult::not_found;
}

std::string ClientAPI::readResultStr(ReadResult res)
{
    switch (res)
    {
        case ReadResult::not_found:
            return "not_found";
        case ReadResult::error:
            return "error";
        case ReadResult::success:
            return "success";
        case ReadResult::client_missing_room:
            return "client_missing_room";
        case ReadResult::failure:
            return "failure";
        case ReadResult::not_implemented:
            return "not_implemented";
        case ReadResult::clean:
            return "clean";
        case ReadResult::unauthorized:
            return "unauthorized";
        case ReadResult::invalid_input:
            return "invalid_input";
    }
    return "not_found";
}

ClientAPI::ReadResult ClientAPI::readMessage(const nx_data& message)
{
    boost::json::object json;
    try
    {
        nx_data messageWithoutLastByte(message.begin(), message.end() - (message.empty() ? 0 : 1));
        json = Json::convertToJSON(messageWithoutLastByte);
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
        return result;
    }
    else
    {
        Log::error("Server returned other than \"success\"");
        Log::debug("ReadResult value: ", readResultStr(result));
        std::string stringMessage = Util::convertToString(message);
        // TODO format output json
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

std::string ClientAPI::createString(const boost::json::value& context, const std::string& key)
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

uint64_t ClientAPI::createUint64(const boost::json::value& context, const std::string& key)
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

float ClientAPI::createFloat(const boost::json::value& context, const std::string& key)
{
    float item = 0.f;
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
    // Capture promise by value to avoid dangling reference.
    return [promise_ptr = std::shared_ptr<std::promise<void>>(&promise, [](auto*) {}), this]()
    {
        try
        {
            // Max 5 seconds waiting time.
            constexpr int max_attempts = 50;
            int attempts = 0;

            while (attempts++ < max_attempts)
            {
                {
                    std::lock_guard<std::mutex> lock(*m_roomsMutex);
                    if (!getActiveRooms().empty())
                    {
                        promise_ptr->set_value();
                        return;
                    }
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }

            // Timeout reached.
            promise_ptr->set_exception(std::make_exception_ptr(
                    std::runtime_error("Timeout waiting for rooms creation")));
        }
        catch (...)
        {
            promise_ptr->set_exception(std::current_exception());
        }
    };
}

Room* ClientAPI::getRoom(uint64_t room_id)
{
    for (auto& room : m_currentlyActiveRooms)
    {
        if (room.getId() == room_id)
        {
            return &room;
        }
    }
    return nullptr;
}

ClientSession* ClientAPI::getClientFromRoom(uint64_t client_id)
{
    // TODO better
    ClientSession* returned_client = nullptr;
    for (auto&& room = m_currentlyActiveRooms.begin(); room != m_currentlyActiveRooms.end(); room++)
    {
        for (auto& client : room->getClients())
        {
            if (client.getId() == client_id)
            {
                returned_client = &client;
            }
        }
    }
    return returned_client;
}

bool ClientAPI::IsInetUDPReady()
{
    return m_clientId && !getInetUDPServerAddress().empty();
}

bool ClientAPI::isInetTCPReady()
{
    return m_clientId && !getInetTCPServerAddress().empty();
}

bool ClientAPI::isBoostTCPReady()
{
    return m_clientId && !getBoostTCPServerAddress().empty();
}

bool ClientAPI::isBoostUDPReady()
{
    return m_clientId && !getBoostUDPServerAddress().empty();
}

bool ClientAPI::isUnixDgramReady()
{
    return m_clientId && !m_data.getUnixDgramServerPath().empty();
}

bool ClientAPI::isUnixStreamReady()
{
    return m_clientId != 0 && !getUnixStreamPath().empty();
}

void ClientAPI::waitUntilInetUDPReady()
{
    while (!IsInetUDPReady())
    {
        Log::debug("ClientAPI waiting for inetUDP to be initialized...");
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void ClientAPI::waitUntilInetTCPReady()
{
    while (!isInetTCPReady())
    {
        Log::debug("ClientAPI waiting for inetTCP to be initialized...");
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void ClientAPI::waitUntilBoostTCPReady()
{
    while (!isBoostTCPReady())
    {
        Log::debug("ClientAPI waiting for BoostTCP to be initialized...");
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void ClientAPI::waitUntilBoostUDPReady()
{
    while (!isBoostUDPReady())
    {
        Log::debug("ClientAPI waiting for BoostUDP to be initialized...");
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void ClientAPI::waitUntilUnixDgramReady()
{
    while (!isUnixDgramReady())
    {
        Log::debug("ClientAPI waiting for unix dgram to be initialized...");
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void ClientAPI::waitUntilUnixStreamReady()
{
    while (!isUnixStreamReady())
    {
        Log::debug("ClientAPI waiting for unix stream to be initialized...");
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}



} // namespace nexilis::client
