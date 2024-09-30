#include <nexilis/command.hh>
#include <nexilis/command_type.hh>
#include <nexilis/client_storage.hh>
#include <nexilis/room_storage.hh>
#include <nexilis/log.hh>

namespace nexilis
{

Authentication* Command::m_authentication = nullptr;

Command::Result Command::read(const char* command_data, size_t length, User& client, Protocol& protocol, uint64_t messageId)
{
    return Command::read(Util::convertToByteVector(command_data, length), client, protocol, messageId);
}

Command::Result Command::read(const std::vector<uint8_t>& command, User& user, Protocol& protocol, uint64_t messageId)
{
    assert(user.getId() != 0);

    Log::debug("Command: Nexilis command sequence");
    Util::debugUint8Vector(command);

    auto arg = command[1];
    switch (static_cast<CommandType>(command.front()))
    {
        case CommandType::setting:
        {
            switch (arg)
            {
                /// Reset your client id.
                /// requires privileges.
                case 0:
                {
                    if (!user.hasRootAccess())
                    {
                        Log::error("Client needs root access for changing id");
                        return Result::unauthorized;
                    }
                    auto payload = Util::removeAmountOfBytesFromVector(command, 2);
                    uint64_t id = Util::convertToType<uint64_t>(payload);

                    auto& clients = ClientStorage::getAllClients();

                    for (auto c = clients.begin(); c != clients.end(); c++)
                    {
                        if (*c == user)
                        {
                            assert(c->hasRootAccess());
                            assert(user.hasRootAccess());
                            c->setId(id);
                            return Result::success;
                        }
                    }

                    Log::error("Error in CommandType::set::clientID");
                    return Result::error;

                }

                // Set username to the client.
                case 1:
                {
                    Log::debug("Called Command::Set::username");

                    // Create string data.
                    auto payload = Util::removeAmountOfBytesFromVector(command, 2);
                    std::string username = Util::convertToString(payload);

                    // Perform server-side operations.
                    auto& clients = ClientStorage::getAllClients();

                    bool setUserName = false;
                    for (auto c = clients.begin(); c != clients.end(); c++)
                    {
                        if (*c == user)
                        {
                            Log::debug("Client username reset!");
                            c->setUsername(username);
                            setUserName = true;
                        }
                    }

                    if (!setUserName)
                    {
                        return Result::error;
                    }

                    // Send data back to "this" client.
                    std::map<std::string, boost::json::value> header{
                                {"command", boost::json::value("set")},
                                {"type", boost::json::value("username")},
                                {"callback", boost::json::value(messageId)},
                                {"username", boost::json::value(username)}};

                    auto data = Util::convertToByteVector(Json::createJSON(header));
                    sendMessageToClient(data, user, protocol);

                    return Result::success;
                }

                default:
                    return Result::not_found;
            }
            return Result::not_found;
        }

        case CommandType::getting:
        {
            switch (arg)
            {
                // Get client id.
                case 0:
                {
                    std::map<std::string, boost::json::value> data{
                            {"command", boost::json::value("get")},
                            {"type", boost::json::value("client_id")},
                            {"client_id", boost::json::value(user.getId())}};

                    auto json = Json::createJSON(data);
                    std::vector<uint8_t> message = Util::convertToByteVector(json);
                    sendMessageToClient(message, user, protocol);

                    Log::info("sent message to client");
                    return Result::success;
                }
                default:
                    return Result::not_found;
            }
        }

        case CommandType::room:
        {
        /**
         *  2:0      Management
         *  2:0:0    Join room; uint64_t roomId
         *  2:0:1    Leave room; void
         *  2:0:2    Create room; string roomName
         *
         *  2:1      Object2D
         *  2:1:0    Set position; Vec2f position
         *  2:1:1    Set dimensions; Vec2f dimensions
         *
         *  2:2    Communication.
         *  2:2:0
         */
            // arg = command[1]
            const uint8_t roomCommandPayloadAmount = 3;
            auto roomArg = command[2];
            switch (arg)
            {
                // Management
                case 0:
                {
                    switch (roomArg)
                    {
                        // Join room.
                        case 0:
                        {
                            Log::debug("Command: Room::Join()");

                            auto payload = Util::removeAmountOfBytesFromVector(command, roomCommandPayloadAmount);
                            uint64_t roomId = Util::convertToType<uint64_t>(payload);
                            auto* room = RoomStorage::getRoomById(roomId);

                            if (!room)
                            {
                                Log::error("Cannot find room with specified id!");
                                return Result::invalid_input;
                            }
                            else
                            {
                                if (RoomStorage::getRoomById(roomId)->contains(user.getId()))
                                {
                                    Log::error("Cannot join room where the client already is!");
                                    return Result::failure;
                                }

                                // Joining room.
                                room->joinRoom(user.getId());
                                user.setRoomId(room->getId());
                                assert(RoomStorage::getRoomById(roomId)->contains(user.getId()));
                                assert(user.getRoomId() == roomId);

                                auto roomId = user.getRoomId();
                                std::map<std::string, boost::json::value> params;
                                return useRooms(roomId, user, protocol, command, params, arg);
                            }
                        }

                        // Leave current room.
                        case 1:
                        {
                            Log::debug("Command Room::leave()");
                            auto* currentRoom = RoomStorage::getRoomById(user.getRoomId());

                            if (!currentRoom)
                            {
                                Log::warning("Client not currently in room so cannot leave current room.");
                                return Result::failure;
                            }

                            currentRoom->leaveRoom(user.getId());
                            assert(!RoomStorage::getRoomById(user.getRoomId())->contains(user.getId()));
                            user.setRoomId(0);

                            std::map<std::string, boost::json::value> params;
                            return useRooms(currentRoom->getId(), user, protocol, command, params, messageId);
                        }

                        /// Create room.
                        case 2:
                        {
                            Log::debug("Command Room::create()");
                            auto payload = Util::removeAmountOfBytesFromVector(command, roomCommandPayloadAmount);
                            std::string roomName = Util::convertToString(payload);

                            if (roomName.empty())
                            {
                                Log::error("Room name cannot be empty");
                                return Result::invalid_input;
                            }
                            else if (roomName == "")
                            {
                                Log::error("Room name cannot be an empty string");
                                return Result::invalid_input;
                            }
                            else if (roomName == " ")
                            {
                                Log::error("Room name cannot be equal to \" \" ");
                                return Result::invalid_input;
                            }
                            else if (roomName.length() > 20)
                            {
                                Log::error("Too long room name");
                                return Result::invalid_input;
                            }
                            else
                            {
                                auto newRoom = Room(Room::Data(user.getId(), roomName));

                                auto newRoomId = newRoom.getId();
                                RoomStorage::add(std::move(newRoom));

                                std::map<std::string, boost::json::value> params;
                                return useRooms(newRoomId, user, protocol, command, params, messageId);

                                Log::debug("Added room ", newRoomId, " to persistent storage");
                                return Result::success;
                            }
                        }
                    }

                    // Object 2D
                    case 1:
                    {
                        switch (roomArg)
                        {
                            /// Position 2D
                            case 0:
                            {
                                Log::debug("Command Room::position2D(vector2)");
                                if (user.getRoomId() == 0)
                                {
                                    Log::error("User not in room!");
                                    return Result::error;
                                }

                                auto payload = Util::removeAmountOfBytesFromVector(command, roomCommandPayloadAmount);
                                auto vector = Util::convertToVector2(payload);
                                Log::debug("Position x:", vector.x, " y:", vector.y);

                                auto currentRoom = RoomStorage::getRoomById(user.getRoomId());

                                if (!currentRoom)
                                {
                                    Log::warning("Client not currently in room.");
                                    return Result::failure;
                                }

                                user.getObject2D().setPosition(vector.x, vector.y);

                                std::map<std::string, boost::json::value> params{
                                        {"x", boost::json::value(vector.x)},
                                        {"y", boost::json::value(vector.y)}};

                                return useRooms(user.getRoomId(), user, protocol, command, params, messageId);
                            }

                            // Dimensions 2D
                            case 1:
                            {
                                Log::debug("Command Room::dimensions(vector2)");
                                auto payload = Util::removeAmountOfBytesFromVector(command, roomCommandPayloadAmount);
                                auto vector = Util::convertToVector2(payload);
                                Log::debug("Dimension x:", vector.x, " y:", vector.y);

                                auto currentRoom = RoomStorage::getRoomById(user.getRoomId());
                                if (!currentRoom)
                                {
                                    Log::error("Client not currently in room.");
                                    return Result::failure;
                                }

                                std::map<std::string, boost::json::value> params{
                                        {"x", boost::json::value(vector.x)},
                                        {"y", boost::json::value(vector.y)}};

                                user.getObject2D().setDimensions(vector.x, vector.y);
                                return useRooms(user.getRoomId(), user, protocol, command, params, messageId);
                            }

                            // Movement 2D
                            case 2:
                            {
                                Log::debug("Command Room::movement(vector2)");
                                auto payload = Util::removeAmountOfBytesFromVector(command, roomCommandPayloadAmount);
                                auto vector = Util::convertToVector2(payload);
                                Log::debug("Movement x: ", vector.x, " y: ", vector.y);

                                auto currentRoom = RoomStorage::getRoomById(user.getRoomId());
                                if (!currentRoom)
                                {
                                    Log::error("Client not currently in room");
                                    return Result::failure;
                                }

                                auto& rooms = RoomStorage::getAllRooms();
                                Vector2f newPos;
                                for (auto& room : rooms)
                                {
                                    if (room.getId() == user.getRoomId())
                                    {
                                        for (auto& roomClient : room.getClients())
                                        {
                                            auto* client = ClientStorage::getClientById(roomClient);
                                            newPos = calculatePosition(user.getObject2D().getPosition(), vector);
                                            client->getObject2D().setPosition(newPos.x, newPos.y);
                                        }
                                    }
                                }

                                std::map<std::string, boost::json::value> params{
                                    {"x", boost::json::value(vector.x)},
                                    {"y", boost::json::value(vector.y)},
                                    {"newPosX", boost::json::value(newPos.x)},
                                    {"newPosY", boost::json::value(newPos.y)}
                                };

                                return useRooms(user.getRoomId(), user, protocol, command, params, messageId);
                            }

                            default: return Result::not_found;
                        }
                    }

                    // Communicate
                    case 2:
                    {
                        return Result::not_found;
                    }
                    default: return Result::not_found;
                }
            }
        }

        case CommandType::authentication:
        {
            switch (arg)
            {
                // Setup authentication.
                case 0:
                {
                    Log::info("New client wants to authenticate, not implemented");
                    return Result::not_found;
                }

                // Check authentication for root access.
                case 1:
                {
                    auto payload = Util::removeAmountOfBytesFromVector(command, 2);
                    std::string password = Util::convertToString(payload);

                    if (m_authentication)
                    {
                        if (m_authentication->isRootPassword(password))
                        {
                            user.setRootAccess(true);
                            Log::info("Client ", user.getIPAddress(), " has root access!");
                            return Result::success;
                        }
                        else
                        {
                            Log::error("Wrong password!");
                            return Result::invalid_input;
                        }
                    }
                    else
                    {
                        Log::error("Trying to set password for server without auth!");
                        return Result::unauthorized;
                    }
                }

                // Passphrase authentication.
                // (Access to join nexilis session)
                case 2:
                {
                    auto payload = Util::removeAmountOfBytesFromVector(command, 2);
                    std::string password = Util::convertToString(payload);

                    if (m_authentication)
                    {
                        if (m_authentication->isPassphrase(password))
                        {
                            user.setCommonAccess(true);
                            Log::info("Client ", user.getIPAddress(), " has common access!");
                            return Result::success;
                        }
                        else
                        {
                            Log::error("Wrong password!");
                            return Result::invalid_input;
                        }
                    }
                    else
                    {
                        Log::error("Trying to set password for server without auth!");
                        return Result::unauthorized;
                    }
                }

                default:
                    return Result::not_found;
            }
        }

        case CommandType::server_management:
        {
            return Result::not_found;
        }

        case CommandType::player_management:
        {
            return Result::not_found;
        }

        case CommandType::error:
        {
            // Internal server error
            switch (arg)
            {
                // Classname X
                case 0:
                {
                    Log::critical("Internal server error: x");
                    return Result::error;
                }
                default:
                    return Result::not_found;
            }
        }

        case CommandType::info:
        {
            switch (arg)
            {
                // Get all public information from a server.
                case 0:
                {
                    std::map<std::string, boost::json::value> header{
                                {"command", boost::json::value("info")},
                                {"type", boost::json::value("server_data")}};

                    auto json = Json::createJSON(header);
                    Json::emplace(json, Json::getServerData());
                    std::vector<uint8_t> data = Util::convertToByteVector(json);

                    sendMessageToClient(data, user, protocol);
                    Log::info("Used Info::generalInfo");
                    return Result::success;
                }

                // Get data from the clients existing on the server.
                case 1:
                {
                    std::map<std::string, boost::json::value> header{
                                {"command", boost::json::value("info")},
                                {"type", boost::json::value("client_data")}};

                    auto json = Json::createJSON(header);
                    Json::emplace(json, Json::getClientData());
                    std::vector<uint8_t> data = Util::convertToByteVector(json);

                    sendMessageToClient(data, user, protocol);
                    Log::info("Used Info::clientInfo");
                    return Result::success;
                }

                // Get data from the rooms existing on the server.
                case 2:
                {
                    std::map<std::string, boost::json::value> header{
                                {"command", boost::json::value("info")},
                                {"type", boost::json::value("room_data")},
                                {"callback", boost::json::value(messageId)}};

                    auto json = Json::createJSON(header);
                    Json::emplace(json, Json::getRoomData());
                    auto data = Util::convertToByteVector(json);
                    sendMessageToClient(data, user, protocol);
                    Log::info("Used Info::roomInfo");
                    return Result::success;
                }
                default: return Result::not_found;
            }
            return Result::not_found;
        }
        default:
            return Result::not_found;
    }
}

void Command::sendMessageToClient(std::vector<uint8_t> data, User& user, Protocol& protocol)
{
    switch (protocol.getType())
    {
        case Protocol::Type::BOOST_TCP_SERVER:
        {
            if (!user.boostTCPSend(data))
            {
                Log::error("Cannot send messages using this protocol (BOOST_TCP)");
            }
            return;
        }

        case Protocol::Type::BOOST_UDP_SERVER:
        {
            if (!user.boostUDPSend(data))
            {
                Log::error("Cannot send messages using this protocol (BOOST_UDP)");
            }
            return;
        }

        case Protocol::Type::BOOST_TCP_CLIENT:
        case Protocol::Type::BOOST_UDP_CLIENT:
        case Protocol::Type::AF_INET_TCP_CLIENT:
        case Protocol::Type::AF_INET_UDP_CLIENT:
        case Protocol::Type::AF_UNIX_SOCK_DGRAM_CLIENT:
        case Protocol::Type::AF_UNIX_SOCK_STREAM_CLIENT:
            Log::error("This function cannot be called with client protocol");
            return;

        default:
        {
            Log::error("Unknown protocol");
            return;
        }
    }
}

Command::Result Command::useRooms(uint64_t roomId, User& user, Protocol& protocol, const std::vector<uint8_t>& messageData, const std::map<std::string, boost::json::value>& params, uint64_t messageId)
{
    RoomType roomType = static_cast<RoomType>(messageData[1]);
    auto action = messageData[2];

    std::string roomCommandAction;
    switch (roomType)
    {
        case RoomType::management:
        {
            roomCommandAction = ManagementTypeToString(static_cast<ManagementOptions>(action));
            break;
        }
        case RoomType::object2D:
        {
            roomCommandAction = Object2DTypeToString(static_cast<Object2DOptions>(action));
            break;
        }
        case RoomType::communication:
        {
            return Result::unimplemented;
        }
    }
    std::string roomCommandType = RoomTypeToString(roomType);
    assert(!roomCommandType.empty());
    assert(!roomCommandAction.empty());

    /// Create message.
    std::map<std::string, boost::json::value> header{
        {"command", boost::json::value("room")},
        {"type", boost::json::value(roomCommandType)},
        {"action", boost::json::value(roomCommandAction)},
        {"roomId", boost::json::value(roomId)},
        {"clientId", boost::json::value(user.getId())},
        {"callback", boost::json::value(messageId)}};
    header.insert(params.begin(), params.end());
    auto json = Json::createJSON(header);
    auto data = Util::convertToByteVector(json);

    auto& rooms = RoomStorage::getAllRooms();
    for (auto& room : rooms)
    {
        if (room.getId() == roomId)
        {
            for (auto& roomClient : room.getClients())
            {
                auto* client = ClientStorage::getClientById(roomClient);
                assert(*client == user);
                sendMessageToClient(data, *client, protocol);
            }
        }
    }
    return Result::success;
}

nexilis::Vector2f Command::calculatePosition(nexilis::Vector2f currentPosition, nexilis::Vector2f velocity)
{
    float deltaTime = 1.0f;
    return {
        currentPosition.x + velocity.x * deltaTime,
        currentPosition.y + velocity.y * deltaTime
    };
}

} // namespace nexilis
