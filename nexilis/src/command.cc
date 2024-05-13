#include <nexilis/command.hh>
#include <nexilis/command_type.hh>
#include <nexilis/client_storage.hh>
#include <nexilis/room_storage.hh>
#include <nexilis/log.hh>

namespace nexilis
{

Authentication* Command::m_authentication = nullptr;

Command::Result Command::read(const char* command_data, size_t lenght, User& client, Protocol& protocol)
{
    return Command::read(Util::convertToByteVector(command_data, lenght), client, protocol);
}

Command::Result Command::read(const std::vector<uint8_t>& command, User& user, Protocol& protocol)
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
                    auto payload = Util::removeAmountOfBytesFromVector(command, 2);
                    std::string username = Util::convertToString(payload);
                    auto& clients = ClientStorage::getAllClients();

                    for (auto c = clients.begin(); c != clients.end(); c++)
                    {
                        if (*c == user)
                        {
                            c->setUsername(username);
                            return Result::success;
                        }
                    }
                    Log::error("Cannot find client");
                    return Result::error;
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
                    // Fix this, nexilis_status = 1 is correct tho.
                    std::map<std::string, boost::json::value> data{
                            {"nexilis_status", boost::json::value(1)},
                            { "type", boost::json::value("set_client_id")},
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
                    auto message = Json::getNexilisStatus(2);
                    Json::emplace(message, Json::getServerData());
                    std::string stringData = boost::json::serialize(message);
                    auto data = Util::convertToByteVector(stringData.c_str(), stringData.size());

                    sendMessageToClient(data, user, protocol);
                    Log::info("Used Info::generalInfo");
                    return Result::success;
                }

                // Get data from the clients existing on the server.
                case 1:
                {
                    auto message = Json::getNexilisStatus(2);
                    Json::emplace(message, Json::getClientDataMessage());
                    std::string stringData = boost::json::serialize(message);
                    auto data = Util::convertToByteVector(stringData.c_str(), stringData.size());

                    sendMessageToClient(data, user, protocol);
                    Log::info("Used Info::clientInfo");
                    return Result::success;
                }

                // Get data from the rooms existing on the server.
                case 2:
                {
                    auto message = Json::getNexilisStatus(2);
                    Json::emplace(message, Json::getRoomDataMessage());
                    std::string stringData = boost::json::serialize(message);
                    auto data = Util::convertToByteVector(stringData.c_str(), stringData.size());

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
                            // TODO start refactoring from "type" to "command".
                            //{"command", boost::json::value("communicate")},
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
                            //{"command", boost::json::value("communicate")},
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
                            //{"command", boost::json::value("communicate")},
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
            Log::debug("MainCommand room");
            Log::debug("Next integer: ", static_cast<int>(command[1]));

            for (uint8_t commandByte : command)
            {
                Log::debug("Commandbyte hex: ", std::hex, static_cast<int>(commandByte));
                Log::debug("Commandbyte char: ", static_cast<char>(commandByte));
            }

            Log::info("Second byte: ", static_cast<int>(command[1]));
            switch (command[1])
            {
                // Join room x.
                case 0:
                {
                    Log::debug("Called Room::Join()");

                    auto payload = Util::removeAmountOfBytesFromVector(command, 2);
                    uint64_t roomId = Util::convertToType<uint64_t>(payload);

                    auto room = RoomStorage::getRoomById(roomId);

                    if (!room)
                    {
                        Log::error("Cannot find room with specified id!");
                        return Result::invalid_input;
                    }
                    else
                    {
                        room->joinRoom(user.getId());
                        user.setRoomId(roomId);
                        assert(RoomStorage::getRoomById(roomId)->contains(user.getId()));
                        return Result::success;
                    }
                }

                // Leave current room.
                case 1:
                {
                    Log::debug("MainCommand room (leave)");
                    auto currentRoom = RoomStorage::getRoomById(user.getRoomId());

                    if (!currentRoom)
                    {
                        Log::warning("Client not currently in room so cannot leave current room.");
                        return Result::failure;
                    }

                    currentRoom->leaveRoom(user.getId());
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

        default:
            return Result::not_found;
    }
}

std::string Command::createIPv4Address(const std::vector<uint8_t>& characters)
{
    if (characters.size() < 4)
    {
        Log::critical("Insufficient characters to create an IPv4 address.");
        return "";
    }

    std::string ipAddress;
    ipAddress += std::to_string(characters[0]) + "." +
                 std::to_string(characters[1]) + "." +
                 std::to_string(characters[2]) + "." +
                 std::to_string(characters[3]);

    return ipAddress;
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
