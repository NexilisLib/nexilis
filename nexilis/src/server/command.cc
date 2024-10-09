#include "nexilis/server/settings.hh"
#include <nexilis/server/client_storage.hh>
#include <nexilis/server/command.hh>
#include <nexilis/server/room_storage.hh>
#include <nexilis/logger/log.hh>

#include <thread>

namespace nexilis
{

Command::Command(const Settings& settings)
    : m_settings(settings)
{
}

Command::Command(Command&& other)
    : m_settings(std::move(other.m_settings))
{
}

Command& Command::operator=(Command&& other)
{
    if (this != &other)
    {
        m_settings = std::move(other.m_settings);
    }
    return *this;
}

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
                                auto roomCommand = createRoomCommand(roomId, user, command, params, messageId);
                                sendRoomCommand(roomCommand, user, protocol);
                                return Result::success;
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
                            auto roomCommand = createRoomCommand(currentRoom->getId(), user, command, params, messageId);
                            sendRoomCommand(roomCommand, user, protocol);
                            return Result::success;
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
                                auto roomCommand = createRoomCommand(newRoomId, user, command, params, messageId);
                                sendRoomCommand(roomCommand, user, protocol);
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

                                auto roomCommand = createRoomCommand(user.getRoomId(), user, command, params, messageId);
                                sendRoomCommand(roomCommand, user, protocol);
                                return Result::success;
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

                                auto roomCommand = createRoomCommand(user.getRoomId(), user, command, params, messageId);
                                sendRoomCommand(roomCommand, user, protocol);
                                return Result::success;
                            }

                            // Movement 2D, creates a thread that sends the new position with time of delta.
                            // In 16 thread CPU: when delta = 0.1f -> ~6 updates.
                            case 2:
                            {
                                Log::debug("Command Room::movement(vector2 movementVector, float delta)");

                                // Get messagedata
                                auto payload = Util::removeAmountOfBytesFromVector(command, roomCommandPayloadAmount);
                                auto vecX = Util::floatFromFront(payload);
                                auto vecY = Util::floatFromFront(Util::removeAmountOfBytesFromVector(payload, 4));
                                auto delta = Util::floatFromFront(Util::removeAmountOfBytesFromVector(payload, 8));
                                auto movementVector = Vector2f(vecX, vecY);
                                auto mtx = std::make_shared<std::mutex>();

                                std::thread([this, mtx, movementVector, &user, command, &protocol, &messageId, delta]()
                                {
                                    try
                                    {
                                        runWithTickrate(m_settings.getTickrate(), delta, [this, &mtx, movementVector, &user, command, &protocol, &messageId](double progress)
                                        {
                                            auto* clientRoom = RoomStorage::getRoomById(user.getRoomId());
                                            assert(clientRoom);

                                            double easedX = easing(progress, movementVector.x);
                                            double easedY = easing(progress, movementVector.y);

                                            Vector2f currentPosition = user.getObject2D().getPosition();
                                            Vector2f dimensions = user.getObject2D().getDimensions();

                                            auto newMovedPosition = Vector2f(easedX + currentPosition.x, easedY + currentPosition.y);
                                            bool limitedMovement = false;
                                            for (auto& c : clientRoom->getClients())
                                            {
                                                User* roomClient = ClientStorage::getClientById(c);

                                                if (roomClient && roomClient->getId() != user.getId())
                                                {
                                                    Vector2f roomClientPosition;
                                                    Vector2f roomClientDimensions;
                                                    {
                                                        std::lock_guard<std::mutex> lock(*mtx);
                                                        roomClientPosition = roomClient->getObject2D().getPosition();
                                                        roomClientDimensions = roomClient->getObject2D().getDimensions();
                                                    }

                                                    // Assumed square.
                                                    if (
                                                            newMovedPosition.x - dimensions.x / 2 < roomClientPosition.x + roomClientDimensions.x / 2 &&
                                                            newMovedPosition.x + dimensions.x / 2 > roomClientPosition.x - roomClientDimensions.x / 2 &&
                                                            newMovedPosition.y - dimensions.y / 2 < roomClientPosition.y + roomClientDimensions.y / 2 &&
                                                            newMovedPosition.y + dimensions.y / 2 > roomClientPosition.y - roomClientDimensions.y / 2
                                                       )
                                                    {
                                                        Log::info("Players tried to hit each other!");
                                                        limitedMovement = true;
                                                        return;
                                                    }
                                                }
                                            }

                                            if (!limitedMovement)
                                            {
                                                {
                                                    std::lock_guard<std::mutex> lock(*mtx);
                                                    user.getObject2D().setPosition(newMovedPosition.x, newMovedPosition.y);
                                                    std::map<std::string, boost::json::value> params = {
                                                        {"x", boost::json::value(newMovedPosition.x)},
                                                        {"y", boost::json::value(newMovedPosition.y)},
                                                    };
                                                    auto roomCommand = createRoomCommand(user.getRoomId(), user, command, params, messageId);
                                                    sendRoomCommand(roomCommand, user, protocol);
                                                }
                                            }
                                        });
                                    }
                                    catch (std::exception& e)
                                    {
                                        Log::error(e.what());
                                    } })
                                        .detach();

                                return Result::success;
                            }

                            default:
                                return Result::not_found;
                        }
                    }

                    // Communicate
                    case 2:
                    {
                        return Result::not_found;
                    }
                    default:
                        return Result::not_found;
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

                    if (m_settings.hasPassphrase())
                    {
                        if (m_settings.isRootPassword(password))
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

                    if (m_settings.hasPassphrase())
                    {
                        if (m_settings.isPassphrase(password))
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
                default:
                    return Result::not_found;
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

std::vector<uint8_t> Command::createRoomCommand(uint64_t roomId, User& user, const std::vector<uint8_t>& messageData, const std::map<std::string, boost::json::value>& params, uint64_t messageId)
{
    if (messageData.size() < 3)
    {
        Log::error("Insuffecient messageData");
        return std::vector<uint8_t>();
    }

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
            Log::error("Unimplemented!");
            return std::vector<uint8_t>();
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
    return Util::convertToByteVector(json);
}

void Command::sendRoomCommand(const std::vector<uint8_t>& data, User& user, Protocol& protocol)
{
    auto& rooms = RoomStorage::getAllRooms();
    for (auto& room : rooms)
    {
        if (room.getId() == user.getRoomId())
        {
            for (auto& roomClient : room.getClients())
            {
                auto* client = ClientStorage::getClientById(roomClient);
                assert(*client == user);
                sendMessageToClient(data, *client, protocol);
            }
        }
    }
}

void Command::runWithTickrate(double tickrate, double durationSeconds, const std::function<void(double)>& tickFunction)
{
    using namespace std::chrono;

    auto interval = duration_cast<milliseconds>(milliseconds(static_cast<int>(1000 / tickrate)));
    auto startTime = steady_clock::now();
    auto endTime = startTime + duration_cast<milliseconds>(milliseconds(static_cast<int>(durationSeconds * 1000)));

    double totalTicks = tickrate * durationSeconds;

    for (int tick = 1; steady_clock::now() <= endTime && tick <= totalTicks; ++tick)
    {
        auto loopStart = steady_clock::now();
        double progress = static_cast<double>(tick) / totalTicks;
        tickFunction(progress);
        std::this_thread::sleep_until(loopStart + interval);
    }
}

double Command::easing(double progress, double totalDistance)
{
    double easedValue = progress * progress;
    double messageValue = totalDistance * easedValue;
    return messageValue;
}

} // namespace nexilis
