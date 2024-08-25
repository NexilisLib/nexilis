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

    switch (static_cast<MainCommand>(command.front()))
    {
        case MainCommand::setting:
        {
            switch (command[1])
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

                    Log::error("Error in MainCommand::set::clientID");
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
                                {"nexilis_status", boost::json::value(1)},
                                {"command", boost::json::value("set")},
                                {"type", boost::json::value("username")},
                                {"callback", boost::json::value(messageId)},
                                {"set_username", boost::json::value(username)}};

                    auto data = Util::convertToByteVector(Json::createJSON(header));
                    sendMessageToClient(data, user, protocol);

                    return Result::success;
                }

                default:
                    return Result::not_found;
            }
            return Result::not_found;
        }

        case MainCommand::getting:
        {
            switch (command[1])
            {
                // Get client id.
                case 0:
                {
                    Log::info("Client id before send: ", user.getId());
                    std::map<std::string, boost::json::value> data{
                            {"nexilis_status", boost::json::value(1)},
                            {"command", boost::json::value("get")},
                            {"type", boost::json::value("set_client_id")},
                            {"set_client_id", boost::json::value(user.getId())}};
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

        // this should be renamed connection management.
        case MainCommand::ping:
        {
            switch (command[1])
            {
                // Send ping.
                case 0:
                {
                    // TODO create pong message, I mean this is kinda stupid.
                    return Result::not_found;
                }

                // Receive ping.
                case 1:
                {
                    return Result::not_found;
                }

                // Start listening
                case 2:
                {
                    return Result::not_found;
                }

                // Stop listening
                case 3:
                {
                    return Result::not_found;
                }

                default:
                {
                    return Result::not_found;
                }
            }
        }
        case MainCommand::info:
        {
            switch (command[1])
            {
                // Get all public information from a server.
                case 0:
                {
                    std::map<std::string, boost::json::value> header{
                                {"nexilis_status", boost::json::value(1)},
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
                                {"nexilis_status", boost::json::value(1)},
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
                                {"nexilis_status", boost::json::value(1)},
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

        case MainCommand::authentication:
        {
            switch (command[1])
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

        case MainCommand::server_management:
        {
            return Result::not_found;
        }

        case MainCommand::player_management:
        {
            return Result::not_found;
        }

        case MainCommand::communicate:
        {
            /// 0, and 1 need some work, running 2 as default.
            switch (command[1])
            {
                /// Send message to every client using the server version of client protocol.
                case 0:
                {
                    auto payload = Util::removeAmountOfBytesFromVector(command, 2);

                    std::map<std::string, boost::json::value> data{
                            {"nexilis_status", boost::json::value(1)},
                            {"command", boost::json::value("communicate")},
                            {"type", boost::json::value("broadcast")},
                            {"message", boost::json::value(Util::convertToString(payload))}};

                    auto json = Json::createJSON(data);
                    std::vector<uint8_t> message = Util::convertToByteVector(json);

                    auto& clients = ClientStorage::getAllClients();
                    for (auto& c : clients)
                    {
                        sendMessageToClient(message, c, protocol);
                    }
                    return Result::success;
                }

                // Client sends a message to everyone except itself.
                case 1:
                {
                    auto payload = Util::removeAmountOfBytesFromVector(command, 2);

                    std::map<std::string, boost::json::value> data{
                            {"nexilis_status", boost::json::value(1)},
                            {"command", boost::json::value("communicate")},
                            {"type", boost::json::value("multicast")},
                            {"message", boost::json::value(Util::convertToString(payload))}};

                    auto json = Json::createJSON(data);
                    std::vector<uint8_t> message = Util::convertToByteVector(json);

                    auto& clients = ClientStorage::getAllClients();

                    for (auto& c : clients)
                    {
                        if (c.getId() != user.getId())
                        {
                            sendMessageToClient(message, c, protocol);
                        }
                    }
                    return Result::success;
                }

                // Client sends a message in a room context.
                case 2:
                {
                    auto payload = Util::removeAmountOfBytesFromVector(command, 2);

                    // Get the room id where client is currently in.
                    auto roomId = user.getRoomId();

                    assert(roomId != 0);

                    std::map<std::string, boost::json::value> data{
                            {"nexilis_status", boost::json::value(1)},
                            {"command", boost::json::value("communicate")},
                            {"type", boost::json::value("room_message")},
                            {"id", boost::json::value(user.getId())},
                            {"roomId", boost::json::value(roomId)},
                            {"message", boost::json::value(Util::convertToString(payload))}};

                    auto json = Json::createJSON(data);
                    std::vector<uint8_t> message = Util::convertToByteVector(json);

                    // Accessing server side clients.
                    auto& clients = ClientStorage::getAllClients();

                    for (auto& c : clients)
                    {
                        /// If the client has the same room id as the sender of the message.
                        if (c.getRoomId() == roomId)
                        {
                            sendMessageToClient(message, c, protocol);
                        }
                    }
                    return Result::success;
                }
            }
            return Result::not_found;
        }

        case MainCommand::error:
        {
            // Internal server error
            switch (command[1])
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

        case MainCommand::room:
        {
            switch (command[1])
            {
                // Join room.
                case 0:
                {
                    Log::debug("Called Room::Join()");

                    auto payload = Util::removeAmountOfBytesFromVector(command, 2);
                    uint64_t roomId = Util::convertToType<uint64_t>(payload);

                    auto* room = RoomStorage::getRoomById(roomId);

                    if (!room)
                    {
                        Log::error("Cannot find room with specified id!");
                        return Result::invalid_input;
                    }
                    else
                    {
                        if (RoomStorage::getRoomById(roomId)->contains(user))
                        {
                            Log::error("Cannot join room where the client already is!");
                            return Result::failure;
                        }

                        room->joinRoom(user);
                        user.setRoomId(roomId);

                        assert(RoomStorage::getRoomById(roomId)->contains(user));
                        assert(user.getRoomId() == roomId);
                        auto roomId = user.getRoomId();

                        std::map<std::string, boost::json::value> header{
                                {"nexilis_status", boost::json::value(1)},
                                {"command", boost::json::value("info")},
                                {"type", boost::json::value("room_data")},
                                {"callback", boost::json::value(messageId)}};

                        auto json = Json::createJSON(header);
                        Json::emplace(json, Json::getRoomData());
                        auto data = Util::convertToByteVector(json);

                        // Accessing server side clients.
                        auto& clients = ClientStorage::getAllClients();

                        for (auto& c : clients)
                        {
                            /// If the client has the same room id as the sender of the message.
                            if (c.getRoomId() == roomId)
                            {
                                sendMessageToClient(data, c, protocol);
                            }
                        }

                        return Result::success;
                    }
                }

                // Leave current room.
                case 1:
                {
                    Log::debug("Called Room::leave()");
                    auto currentRoom = RoomStorage::getRoomById(user.getRoomId());

                    if (!currentRoom)
                    {
                        Log::warning("Client not currently in room so cannot leave current room.");
                        return Result::failure;
                    }

                    currentRoom->leaveRoom(user);
                    assert(!RoomStorage::getRoomById(user.getRoomId())->contains(user));
                    user.setRoomId(0);

                    std::map<std::string, boost::json::value> header{
                            {"nexilis_status", boost::json::value(1)},
                            {"command", boost::json::value("info")},
                            {"type", boost::json::value("room_data")},
                            {"callback", boost::json::value(messageId)}};

                    auto json = Json::createJSON(header);
                    Json::emplace(json, Json::getRoomData());
                    auto data = Util::convertToByteVector(json);

                    // Accessing server side clients.
                    auto& clients = ClientStorage::getAllClients();

                    for (auto& c : clients)
                    {
                        // Send message to all clients existing in this room.
                        if (c.getRoomId() == currentRoom->getId())
                        {
                            sendMessageToClient(data, c, protocol);
                        }
                    }

                    return Result::success;
                }

                /// Create room.
                case 2:
                {
                    Log::debug("MainCommand room (create)");
                    auto payload = Util::removeAmountOfBytesFromVector(command, 2);
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
                        Log::debug("Added room ", newRoomId, " to persistent storage");
                        return Result::success;
                    }
                }

                default: return Result::not_found;
            }
        }

        case MainCommand::position:
        {
            switch (command[1])
            {
                case 0:
                {
                    Log::debug("MainCommand position (vector2)");
                    auto payload = Util::removeAmountOfBytesFromVector(command, 2);
                    auto vector = Util::convertToVector2(payload);
                    Log::debug("Position x:", vector.x, " y:", vector.y);

                    std::map<std::string, boost::json::value> data{
                            {"nexilis_status", boost::json::value(1)},
                            {"command", boost::json::value("position")},
                            {"type", boost::json::value("vector2")},
                            {"positionX", boost::json::value(vector.x)},
                            {"positionY", boost::json::value(vector.y)}};

                    auto json = Json::createJSON(data);
                    std::vector<uint8_t> message = Util::convertToByteVector(json);
                    sendMessageToClient(message, user, protocol);
                    return Result::success;
                }
                default: return Result::not_found;
            }
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

} // namespace nexilis
