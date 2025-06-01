#include <nexilis/json.hh>
#include <nexilis/logger/log.hh>
#include <nexilis/movement_type.hh>
#include <nexilis/room_command_type.hh>
#include <nexilis/server/client_storage.hh>
#include <nexilis/server/command.hh>
#include <nexilis/server/room_storage.hh>
#include <nexilis/server/server_json.hh>

#include <thread>

namespace nexilis::server
{

std::string Command::resultTypeAsString(Result res)
{
    switch (res)
    {
        case Result::error:
            return "error";
        case Result::failure:
            return "failure";
        case Result::invalid_input:
            return "invalid_input";
        case Result::success:
            return "success";
        case Result::not_found:
            return "not_found";
        case Result::unauthorized:
            return "unauthorized";
        case Result::unimplemented:
            return "unimplemented";
    }
    return "";
}

Command::Command(const Settings& settings)
    : NxClass("server::Command"),
      m_settings(settings)
{
}

Command::Command(Command&& other)
    : NxClass(std::move(other)),
      m_settings(std::move(other.m_settings))
{
}

Command& Command::operator=(Command&& other)
{
    if (this != &other)
    {
        m_settings = std::move(other.m_settings);
        NxClass::operator=(std::move(other));
    }
    return *this;
}

bool Command::checkResult(Result result)
{
    switch (result)
    {
        case Result::success:
            return true;

        case Result::unauthorized:
            Log::error("Unauthorized");
            break;

        case Result::unimplemented:
            Log::error("Unimplemented");
            break;

        case Result::not_found:
            Log::error("Not found");
            break;

        case Result::invalid_input:
            Log::error("Invalid input");
            break;

        case Result::error:
            Log::error("Error");
            break;

        case Result::failure:
            Log::error("Failure");
            break;
    }
    return false;
}

Command::Result Command::read(const char* command_data, size_t length, User& client, Protocol& protocol, uint64_t messageId)
{
    return Command::read(Util::convertToByteVector(command_data, length), client, protocol, messageId);
}

Command::Result Command::read(const nx_data& command, User& user, Protocol& protocol, uint64_t messageId)
{
    assert(user.getId() != 0);

    Log::debug("Command: Nexilis command sequence");
    Util::debugUint8Vector(command);

    // The first byte.
    auto main_arg = static_cast<CommandType>(command.front());

    // The second byte.
    auto arg = command[1];

    Log::debug(header(), commandTypeAsString(main_arg));
    Log::debug(header(), "Arg:", arg);

    switch (main_arg)
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
                    // We are parsing Command, so remove two bytes from this switch statement.
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
                    auto payload = Util::removeAmountOfBytesFromVector(command, 2);
                    std::string username = Util::convertToString(payload);

                    auto* client = ClientStorage::getClientById(user.getId());
                    if (client)
                    {
                        client->setUsername(username);
                    }
                    else
                    {
                        return Result::error;
                    }

                    auto data = clientMessageData(CommandType::setting, "username", messageId, {{"username", boost::json::value(username)}});
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
                            {"command", boost::json::value("getting")},
                            {"type", boost::json::value("client_id")},
                            {"client_id", boost::json::value(user.getId())}};

                    auto json = Json::createJSON(data);
                    nx_data message = Util::convertToByteVector(json);
                    sendMessageToClient(message, user, protocol);

                    Log::info(header(), "Sent message GET CLIENTID to client");
                    return Result::success;
                }
                default:
                    return Result::not_found;
            }
        }

        case CommandType::room:
        {
            // Payload for room commands start after the third byte.
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

                                std::map<std::string, boost::json::value> params;
                                auto roomCommand = createRoomCommand(roomId, user, command, params, messageId);
                                sendRoomCommand(roomCommand, user, protocol);
                                return Result::success;
                            }
                        }

                        // Leave current room.
                        case 1:
                        {
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
                            auto payload = Util::removeAmountOfBytesFromVector(command, roomCommandPayloadAmount);
                            uint8_t context = payload[0];
                            std::string roomName = Util::convertToString(Util::removeAmountOfBytesFromVector(payload, 1));

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
                                auto newRoom = Room(RoomData(user.getId(), roomName, Util::getRandomUint64(), static_cast<RoomData::Context>(context)));

                                auto newRoomId = newRoom.getId();
                                RoomStorage::add(std::move(newRoom));

                                std::map<std::string, boost::json::value> params;
                                auto roomCommand = createRoomCommand(newRoomId, user, command, params, messageId);
                                sendRoomCommand(roomCommand, user, protocol);
                                return Result::success;
                            }
                        }
                    }
                    // Communicate
                    case 1:
                    {
                        switch (roomArg)
                        {
                            // broadcast
                            case 0:
                            {
                                // Get messagedata.
                                auto payload = Util::removeAmountOfBytesFromVector(command, roomCommandPayloadAmount);
                                auto messageData = Util::convertToString(payload);

                                if (user.getRoomId() == 0)
                                {
                                    Log::error("User not currently in room!");
                                    return Result::error;
                                }

                                std::map<std::string, boost::json::value> params{
                                        {"id", boost::json::value(user.getId())},
                                        {"roomId", boost::json::value(user.getRoomId())},
                                        {"message", boost::json::value(messageData)}};

                                auto roomCommand = createRoomCommand(user.getRoomId(), user, command, params, messageId);
                                sendRoomCommand(roomCommand, user, protocol);
                                return Result::success;
                            }

                            // othercast
                            case 1:
                            {
                                return Result::unimplemented;
                            }

                            // unicast
                            case 2:
                            {
                                return Result::unimplemented;
                            }
                        }
                    }
                    default:
                        return Result::not_found;
                }
                // Player 2D
                case 2:
                {
                    switch (roomArg)
                    {
                        /// Position 2D
                        case 0:
                        {
                            if (user.getRoomId() == 0)
                            {
                                Log::error(header(), "User not in room!");
                                return Result::error;
                            }

                            auto payload = Util::removeAmountOfBytesFromVector(command, roomCommandPayloadAmount);
                            auto vector = Util::convertToVector2(payload);
                            Log::debug(header(), "Position x:", vector.x, " y:", vector.y);

                            auto currentRoom = RoomStorage::getRoomById(user.getRoomId());

                            if (!currentRoom)
                            {
                                Log::warning(header(), "Client not currently in room.");
                                return Result::failure;
                            }

                            user.getObject2D().setPosition(vector);

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
                            auto payload = Util::removeAmountOfBytesFromVector(command, roomCommandPayloadAmount);
                            auto vector = Util::convertToVector2(payload);
                            Log::debug(header(), "Dimension x:", vector.x, " y:", vector.y);

                            auto currentRoom = RoomStorage::getRoomById(user.getRoomId());
                            if (!currentRoom)
                            {
                                Log::error("Client not currently in room.");
                                return Result::failure;
                            }

                            std::map<std::string, boost::json::value> params{
                                    {"x", boost::json::value(vector.x)},
                                    {"y", boost::json::value(vector.y)}};

                            user.getObject2D().setDimensions(vector);

                            auto roomCommand = createRoomCommand(user.getRoomId(), user, command, params, messageId);
                            sendRoomCommand(roomCommand, user, protocol);
                            return Result::success;
                        }

                        // Movement 2D, creates a thread that sends the new position with time of delta.
                        // In 16 thread CPU: when delta = 0.1f -> ~6 updates.
                        case 2:
                        {
                            // Get messagedata
                            auto payload = Util::removeAmountOfBytesFromVector(command, roomCommandPayloadAmount);
                            auto vecX = Util::floatFromFront(payload);
                            auto vecY = Util::floatFromFront(Util::removeAmountOfBytesFromVector(payload, 4));
                            auto delta = Util::floatFromFront(Util::removeAmountOfBytesFromVector(payload, 8));
                            auto movementVector = Vector2f(vecX, vecY);
                            auto mtx = std::make_shared<std::mutex>();

                            // clang-format off
                            std::thread([this, mtx, movementVector, &user, command, &protocol, &messageId, delta]()
                            {
                                try
                                {
                            // clang-format on
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
                                                user.getObject2D().setPosition(newMovedPosition);
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

                // Object2D
                case 3:
                {
                    switch (roomArg)
                    {
                        // Create object
                        case 0:
                        {
                            // Get messagedata
                            auto payload = Util::removeAmountOfBytesFromVector(command, roomCommandPayloadAmount);
                            auto position = Util::vector2fFromFront(payload);
                            auto dimensions = Util::vector2fFromFront(Util::removeAmountOfBytesFromVector(payload, 8));
                            auto fileBytes = Util::removeAmountOfBytesFromVector(payload, 16);
                            auto filepath = Util::convertToString(fileBytes);

                            // Create server object.
                            auto object = Object2D(Util::getRandomUint64(), position, dimensions);
                            object.setFilepath(filepath);
                            uint64_t objectId = object.getId();
                            Log::info("Created object with id: ", objectId);

                            // Add to storage.
                            auto room = RoomStorage::getRoomById(user.getRoomId());
                            room->addObject(std::move(object));

                            std::map<std::string, boost::json::value> params{
                                    {"positionX", boost::json::value(position.x)},
                                    {"positionY", boost::json::value(position.y)},
                                    {"dimensionX", boost::json::value(dimensions.x)},
                                    {"dimensionY", boost::json::value(dimensions.y)},
                                    {"filepath", boost::json::value(filepath)},
                                    {"id", boost::json::value(objectId)}};

                            auto roomCommand = createRoomCommand(user.getRoomId(), user, command, params, messageId);
                            sendRoomCommand(roomCommand, user, protocol);
                            return Result::success;
                        }

                        case 1:
                        {
                            auto payload = Util::removeAmountOfBytesFromVector(command, roomCommandPayloadAmount);
                            auto objectId = Util::uint64FromFront(payload);

                            // Remove from storage.
                            auto room = RoomStorage::getRoomById(user.getRoomId());
                            room->deleteObject2D(objectId);

                            std::map<std::string, boost::json::value> params{
                                    {"id", boost::json::value(objectId)}};

                            auto roomCommand = createRoomCommand(user.getRoomId(), user, command, params, messageId);
                            sendRoomCommand(roomCommand, user, protocol);
                            return Result::success;
                        }

                        // Move object
                        case 2:
                        {
                            // Get messagedata.
                            auto payload = Util::removeAmountOfBytesFromVector(command, roomCommandPayloadAmount);
                            auto objectId = Util::uint64FromFront(payload);
                            auto position = Util::vector2fFromFront(Util::removeAmountOfBytesFromVector(payload, 8));

                            // Get object from server storage.
                            auto room = RoomStorage::getRoomById(user.getRoomId());
                            if (!room)
                            {
                                Log::error("Client room not found!");
                                return Result::failure;
                            }

                            auto object = room->getObject2DById(objectId);
                            if (!object)
                            {
                                Log::error("Object not found with id: ", objectId);
                                return Result::failure;
                            }

                            auto oldPosition = object->getPosition();
                            auto newPosition = oldPosition + position;

                            std::map<std::string, boost::json::value> params{
                                    {"objectId", boost::json::value(objectId)},
                                    {"x", boost::json::value(newPosition.x)},
                                    {"y", boost::json::value(newPosition.y)}};

                            // Move object in server storage.
                            object->setPosition(newPosition);

                            auto roomCommand = createRoomCommand(user.getRoomId(), user, command, params, messageId);
                            sendRoomCommand(roomCommand, user, protocol);
                            return Result::success;
                        }

                        // Create moving object
                        case 3:
                        {
                            // Get messagedata.
                            auto payload = Util::removeAmountOfBytesFromVector(command, roomCommandPayloadAmount);
                            auto startingPosition = Util::vector2fFromFront(payload);
                            auto dimensions = Util::vector2fFromFront(Util::removeAmountOfBytesFromVector(payload, 8));
                            auto movement = Util::vector2fFromFront(Util::removeAmountOfBytesFromVector(payload, 16));
                            auto deltaTime = Util::floatFromFront(Util::removeAmountOfBytesFromVector(payload, 24));
                            auto movementType = static_cast<MovementType>(payload[28]);
                            auto fileBytes = Util::removeAmountOfBytesFromVector(payload, 29);
                            auto filepath = Util::convertToString(fileBytes);

                            // Create new object.
                            auto object = Object2D(Util::getRandomUint64(), startingPosition, dimensions);
                            object.setFilepath(filepath);
                            uint64_t objectId = object.getId();
                            Log::info("Created object with id: ", objectId);

                            // Add newly created object to storage.
                            auto room = RoomStorage::getRoomById(user.getRoomId());
                            room->addObject(std::move(object));

                            std::map<std::string, boost::json::value> params{
                                    {"createMovingType", boost::json::value("create")},
                                    {"positionX", boost::json::value(startingPosition.x)},
                                    {"positionY", boost::json::value(startingPosition.y)},
                                    {"dimensionX", boost::json::value(dimensions.x)},
                                    {"dimensionY", boost::json::value(dimensions.y)},
                                    {"filepath", boost::json::value(filepath)},
                                    {"id", boost::json::value(objectId)}};
                            auto roomCommand = createRoomCommand(user.getRoomId(), user, command, params, messageId);
                            sendRoomCommand(roomCommand, user, protocol);

                            std::function<double(double, double)> movementFunction;
                            switch (movementType)
                            {
                                case MovementType::eased:
                                    movementFunction = [this](double progress, double totalDistance) -> double
                                    {
                                        return this->easing(progress, totalDistance);
                                    };
                                    break;
                                case MovementType::linear:
                                    movementFunction = [this](double progress, double totalDistance) -> double
                                    {
                                        return this->linear(progress, totalDistance);
                                    };
                                    break;
                                default:
                                    Log::error(header(), "Undefined movement type!");
                            }

                            auto base_move = std::make_unique<Movement2D>(MovementData(objectId, deltaTime, command, messageId), movement, movementFunction);
                            object2DMovement(std::move(base_move), user, protocol).detach();
                            return Result::success;
                        }

                        default:
                            return Result::not_found;
                    }
                }
                // Player3D
                case 4:
                {
                    switch (roomArg)
                    {
                        /// Position 3D
                        case 0:
                        {
                            if (user.getRoomId() == 0)
                            {
                                Log::error(header(), "User not in room!");
                                return Result::error;
                            }

                            auto payload = Util::removeAmountOfBytesFromVector(command, roomCommandPayloadAmount);
                            assert(payload.size() == 12);
                            auto vector = Util::convertToVector3(payload);
                            Log::debug(header(), "Position x:", vector.x, " y:", vector.y, " z:", vector.z);

                            auto currentRoom = RoomStorage::getRoomById(user.getRoomId());

                            if (!currentRoom)
                            {
                                Log::warning(header(), "Client not currently in room.");
                                return Result::failure;
                            }

                            user.getObject3D().setPosition(vector);

                            std::map<std::string, boost::json::value> params{
                                    {"x", boost::json::value(vector.x)},
                                    {"y", boost::json::value(vector.y)},
                                    {"z", boost::json::value(vector.z)}};

                            auto roomCommand = createRoomCommand(user.getRoomId(), user, command, params, messageId);
                            sendRoomCommand(roomCommand, user, protocol);
                            return Result::success;
                        }

                        // Dimensions 3D
                        case 1:
                        {
                            auto payload = Util::removeAmountOfBytesFromVector(command, roomCommandPayloadAmount);
                            auto vector = Util::convertToVector3(payload);
                            Log::debug("Dimension x:", vector.x, " y:", vector.y, " z:", vector.z);

                            auto currentRoom = RoomStorage::getRoomById(user.getRoomId());
                            if (!currentRoom)
                            {
                                Log::error("Client not currently in room.");
                                return Result::failure;
                            }

                            std::map<std::string, boost::json::value> params{
                                    {"x", boost::json::value(vector.x)},
                                    {"y", boost::json::value(vector.y)},
                                    {"z", boost::json::value(vector.z)}};

                            user.getObject3D().setDimensions(vector);

                            auto roomCommand = createRoomCommand(user.getRoomId(), user, command, params, messageId);
                            sendRoomCommand(roomCommand, user, protocol);
                            return Result::success;
                        }
                        default:
                            return Result::not_found;
                    }
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
                            {"type", boost::json::value("server_data")},
                            {"callback", boost::json::value(messageId)}};

                    auto json = Json::createJSON(header);
                    Json::emplace(json, ServerJson::getServerData());
                    nx_data data = Util::convertToByteVector(json);

                    sendMessageToClient(data, user, protocol);
                    Log::info("Used Info::generalInfo");
                    return Result::success;
                }

                // Get data from the clients existing on the server.
                case 1:
                {
                    std::map<std::string, boost::json::value> header{
                            {"command", boost::json::value("info")},
                            {"type", boost::json::value("client_data")},
                            {"callback", boost::json::value(messageId)}};

                    auto json = Json::createJSON(header);
                    Json::emplace(json, ServerJson::getClientData());
                    nx_data data = Util::convertToByteVector(json);

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
                    Json::emplace(json, ServerJson::getRoomData());
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

void Command::sendMessageToClient(nx_data data, User& user, Protocol& protocol)
{
    switch (protocol.getType())
    {
        case Protocol::Type::BOOST_TCP_SERVER:
        {
            if (!user.boostTCPSend(data))
            {
                Log::error(header(), "Cannot send messages using (BOOST_TCP)");
            }
            return;
        }

        case Protocol::Type::BOOST_UDP_SERVER:
        {
            if (!user.boostUDPSend(data))
            {
                Log::error(header(), "Cannot send messages using (BOOST_UDP)");
            }
            return;
        }

        case Protocol::Type::AF_UNIX_SOCK_STREAM_SERVER:
        {
            if (!user.unixStreamSend(data))
            {
                Log::error(header(), "Cannot send messages using (", protocol.typeToString(protocol.getType()), ")");
            }
            return;
        }

        case Protocol::Type::BOOST_TCP_CLIENT:
        case Protocol::Type::BOOST_UDP_CLIENT:
        case Protocol::Type::AF_INET_TCP_CLIENT:
        case Protocol::Type::AF_INET_UDP_CLIENT:
        case Protocol::Type::AF_UNIX_SOCK_DGRAM_CLIENT:
        case Protocol::Type::AF_UNIX_SOCK_STREAM_CLIENT:
            Log::error(header(), "This function cannot be called with client protocol");
            return;

        default:
        {
            Log::error(header(), "Cannot send messages using (", protocol.typeToString(protocol.getType()), ")");
            return;
        }
    }
}

nx_data Command::createRoomCommand(uint64_t roomId, User& user, const nx_data& messageData, const std::map<std::string, boost::json::value>& params, uint64_t messageId)
{
    if (messageData.size() < 3)
    {
        Log::error(header(), "Insuffecient messageData");
        return nx_data();
    }

    auto roomType = static_cast<RoomCommandType::Root>(messageData[1]);
    auto action = messageData[2];

    std::string roomCommandAction;
    switch (roomType)
    {
        case RoomCommandType::Root::management:
        {
            roomCommandAction = RoomCommandType::ManagementTypeToString(static_cast<RoomCommandType::Management>(action));
            break;
        }
        case RoomCommandType::Root::player2D:
        {
            roomCommandAction = RoomCommandType::PlayerTypeToString(static_cast<RoomCommandType::Player2D>(action));
            break;
        }
        case RoomCommandType::Root::object2D:
        {
            roomCommandAction = RoomCommandType::ObjectTypeToString(static_cast<RoomCommandType::Object2D>(action));
            break;
        }
        case RoomCommandType::Root::player3D:
        {
            roomCommandAction = RoomCommandType::PlayerTypeToString(static_cast<RoomCommandType::Player3D>(action));
            break;
        }
        case RoomCommandType::Root::object3D:
        {
            roomCommandAction = RoomCommandType::ObjectTypeToString(static_cast<RoomCommandType::Object3D>(action));
            break;
        }
        case RoomCommandType::Root::communication:
        {
            roomCommandAction = RoomCommandType::CommunicationTypeToString(static_cast<RoomCommandType::Communication>(action));
            break;
        }
        case RoomCommandType::Root::undefined:
        {
            Log::error("Command: Undefined roomCommandAction");
            break;
        }
    }
    std::string roomCommandType = RoomCommandType::RoomTypeToString(roomType);
    assert(!roomCommandType.empty());
    assert(!roomCommandAction.empty());

    auto all_params = ClientMsgType{
            {"action", boost::json::value(roomCommandAction)},
            {"roomId", boost::json::value(roomId)},
            {"clientId", boost::json::value(user.getId())},
    };
    all_params.insert(params.begin(), params.end());

    return clientMessageData(CommandType::room, roomCommandType, messageId, all_params);
}

void Command::sendRoomCommand(const nx_data& data, User& user, Protocol& protocol)
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

double Command::linear(double progress, double totalDistance)
{
    return totalDistance * progress;
}

std::thread Command::object2DMovement(std::unique_ptr<Movement2D> movement, User& user, Protocol& protocol)
{
    // clang-format off
    return std::thread([this, &movement, &user, &protocol]()
    {
    // clang-format on
        try
        {
            runWithTickrate(m_settings.getTickrate(), movement->getDeltatime(), [this, &movement, &user, &protocol](double progress)
            {
                auto func = movement->getMovementFunc();
                auto amount = movement->getAmount();

                auto* room = RoomStorage::getRoomById(user.getRoomId());
                auto* serverObject = room->getObject2DById(movement->getObjectId());

                double movementY = func(progress, amount.y);
                double movementX = func(progress, amount.x);

                Vector2f currentPosition = serverObject->getPosition();
                Vector2f newPosition = Vector2f(currentPosition.x + movementX, currentPosition.y + movementY);

                serverObject->setPosition(newPosition);
                std::map<std::string, boost::json::value> messageParams
                {
                    {"createMovingType", boost::json::value("update")},
                    {"objectId", boost::json::value(serverObject->getId())},
                    {"x", boost::json::value(newPosition.x)},
                    {"y", boost::json::value(newPosition.y)}
                };
                auto roomCommand = createRoomCommand(user.getRoomId(), user, movement->getMessageData(), messageParams, movement->getMessageId());
                sendRoomCommand(roomCommand, user, protocol);
            });
        }
        catch (const std::exception& e)
        {
            Log::error(e.what());
        } });
}

nx_data Command::clientMessageData(CommandType cmd, const std::string& type, uint64_t message_id, const ClientMsgType& params)
{
    return Util::convertToByteVector(Json::createJSON(clientMessageMap(cmd, type, message_id, params)));
}

Command::ClientMsgType Command::clientMessageMap(CommandType cmd, const std::string& type, uint64_t message_id, const ClientMsgType& params)
{
    auto data = ClientMsgType{
            {"command", boost::json::value(commandTypeAsString(cmd))},
            {"type", boost::json::value(type)},
            {"callback", boost::json::value(message_id)}};
    data.insert(params.begin(), params.end());
    return data;
}

} // namespace nexilis::server
