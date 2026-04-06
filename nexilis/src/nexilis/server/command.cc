#include <nexilis/json.hh>
#include <nexilis/logger/log.hh>
#include <nexilis/movement_type.hh>
#include <nexilis/room_command_type.hh>
#include <nexilis/server/client_storage.hh>
#include <nexilis/server/command/command.hh>
#include <nexilis/server/command/commands.hh>
#include <nexilis/server/movement.hh>
#include <nexilis/server/room_storage.hh>
#include <nexilis/server/server_json.hh>

namespace nexilis::server
{

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

CommandResult Command::read(const char* command_data, size_t length, User& client, Protocol& protocol, uint64_t messageId)
{
    return Command::read(Util::convertToByteVector(command_data, length), client, protocol, messageId);
}

CommandResult Command::read(const nx_data& command, User& user, Protocol& protocol, uint64_t messageId)
{
    assert(user.getId() != 0);

    Log::debug("Command: Nexilis command sequence");
    Util::debugUint8Vector(command);

    // The first byte.
    auto arg = static_cast<CommandType>(command.front());

    // The second byte.
    auto arg2 = command[1];

    uint8_t arg3 = 255, arg4 = 255;

    if (command.size() > 2)
    {
        arg3 = command[2];
    }
    if (command.size() > 3)
    {
        arg4 = command[3];
    }

    Commands::DefaultArgs args(user, protocol, command, messageId);

    Log::debug(header(), commandTypeAsString(arg));
    switch (arg)
    {
        case CommandType::setting:
        {
            switch (arg2)
            {
                // General settings.
                case 0:
                {
                    switch (arg3)
                    {
                        // Reset your client id.
                        // requires privileges.
                        case 0:
                        {
                            return Commands::Set::General::clientId(user, command);
                        }

                        // Set username to the client.
                        case 1:
                        {
                            return Commands::Set::General::username(args);
                        }
                    }
                    return CommandResult::not_found;
                }

                // Protocol specific setting
                case 1:
                {
                    switch (arg3)
                    {
                        // Boost TCP
                        case 0:
                        {
                            switch (arg4)
                            {
                                // Server address
                                case 0:
                                {
                                    Log::debug(header(), "setting::protocol::boostTCP::server_address");
                                    return CommandResult::unimplemented;
                                }

                                // Server port number
                                case 1:
                                {
                                    return Commands::Set::Protocol::BoostTCP::port(args);
                                }
                            }
                        }
                    }
                    return CommandResult::not_found;
                }
            }
            return CommandResult::not_found;
        }

        case CommandType::getting:
        {
            switch (arg2)
            {
                // General getting
                case 0:
                {
                    switch (arg3)
                    {
                        // Get client id.
                        case 0:
                        {
                            return Commands::Get::General::clientId(args);
                        }
                    }
                }
            }
            return CommandResult::not_found;
        }

        case CommandType::room:
        {
            switch (arg2)
            {
                // Management
                case 0:
                {
                    switch (arg3)
                    {
                        // Join room.
                        case 0:
                        {
                            return Commands::Room::Management::join(args);
                        }

                        // Leave current room.
                        case 1:
                        {
                            return Commands::Room::Management::leave(args);
                        }

                        /// Create room.
                        case 2:
                        {
                            return Commands::Room::Management::create(args);
                        }
                    }
                    return CommandResult::not_found;
                }
                // Communicate
                case 1:
                {
                    switch (arg3)
                    {
                        // broadcast
                        case 0:
                        {
                            return Commands::Room::Communicate::broadcast(args);
                        }

                        // othercast
                        case 1:
                        {
                            return CommandResult::unimplemented;
                        }

                        // unicast
                        case 2:
                        {
                            return CommandResult::unimplemented;
                        }
                    }
                    return CommandResult::not_found;
                }
                // Player 2D
                case 2:
                {
                    switch (arg3)
                    {
                        // Position 2D
                        case 0:
                        {
                            return Commands::Room::Player2D::position(args);
                        }

                        // Dimensions 2D
                        case 1:
                        {
                            return Commands::Room::Player2D::dimension(args);
                        }

                        // Movement 2D
                        case 2:
                        {
                            return Commands::Room::Player2D::movement(args);
                        }
                    }
                    return CommandResult::not_found;
                }

                // Object2D
                case 3:
                {
                    switch (arg3)
                    {
                        // Create object
                        case 0:
                        {
                            return Commands::Room::Object2D::create(args);
                        }

                        // Delete object
                        case 1:
                        {
                            return Commands::Room::Object2D::remove(args);
                        }

                        // Move object
                        case 2:
                        {
                            return Commands::Room::Object2D::move(args);
                        }

                        // Create moving object
                        case 3:
                        {
                            return Commands::Room::Object2D::createMoving(args);
                        }
                    }
                    return CommandResult::not_found;
                }
                // Player3D
                case 4:
                {
                    switch (arg3)
                    {
                        /// Position 3D
                        case 0:
                        {
                            return Commands::Room::Player3D::position(args);
                        }

                        // Dimensions 3D
                        case 1:
                        {
                            return Commands::Room::Player3D::dimension(args);
                        }

                        // Movement 3D
                        case 2:
                        {
                            return Commands::Room::Player3D::movement(args);
                        }
                    }
                }
            }
            return CommandResult::not_found;
        }

        case CommandType::authentication:
        {
            switch (arg2)
            {
                // Setup authentication.
                case 0:
                {
                    Log::info("New client wants to authenticate, not implemented");
                    return CommandResult::not_found;
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
                            return CommandResult::success;
                        }
                        else
                        {
                            Log::error("Wrong password!");
                            return CommandResult::invalid_input;
                        }
                    }
                    else
                    {
                        Log::error("Trying to set password for server without auth!");
                        return CommandResult::unauthorized;
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
                            return CommandResult::success;
                        }
                        else
                        {
                            Log::error("Wrong password!");
                            return CommandResult::invalid_input;
                        }
                    }
                    else
                    {
                        Log::error("Trying to set password for server without auth!");
                        return CommandResult::unauthorized;
                    }
                }
            }
            return CommandResult::not_found;
        }

        case CommandType::server_management:
        {
            return CommandResult::not_found;
        }

        case CommandType::player_management:
        {
            return CommandResult::not_found;
        }

        case CommandType::error:
        {
            // Internal server error
            switch (arg2)
            {
                // Classname X
                case 0:
                {
                    Log::critical("Internal server error: x");
                    return CommandResult::error;
                }
            }
            return CommandResult::not_found;
        }

        case CommandType::info:
        {
            switch (arg2)
            {
                // Get all public information from a server.
                case 0:
                {
                    Log::debug(header(), "info::server_data");
                    auto server_data = ServerJson::getServerData();
                    Command::ClientMsgType params;
                    for (const auto& [key, value] : server_data)
                    {
                        params[key] = value;
                    }
                    auto data = clientMessageData(CommandType::info, "server_data", messageId, params);
                    sendMessageToClient(data, user, protocol);
                    Log::info("Used Info::generalInfo");
                    return CommandResult::success;
                }

                // Get data from the clients existing on the server.
                case 1:
                {
                    Log::debug(header(), "info::client_data");
                    auto client_data = ServerJson::getClientData();
                    Command::ClientMsgType params;
                    for (const auto& [key, value] : client_data)
                    {
                        params[key] = value;
                    }
                    auto data = clientMessageData(CommandType::info, "client_data", messageId, params);
                    sendMessageToClient(data, user, protocol);

                    Log::info("Used Info::clientInfo");
                    return CommandResult::success;
                }

                // Get data from the rooms existing on the server.
                case 2:
                {
                    Log::debug(header(), "info::room_data");

                    auto roomData = ServerJson::getRoomData();
                    Command::ClientMsgType params;
                    for (const auto& [key, value] : roomData)
                    {
                        params[key] = value;
                    }
                    auto data = clientMessageData(CommandType::info, "room_data", messageId, params);
                    sendMessageToClient(data, user, protocol);

                    Log::info("Used Info::roomInfo");
                    return CommandResult::success;
                }
            }
            return CommandResult::not_found;
        }
        case CommandType::undefined:
        {
            return CommandResult::error;
        }
    }
    return CommandResult::not_found;
}

bool Command::sendMessageToClient(nx_data data, User& user, Protocol& protocol)
{
    auto check = [&protocol](bool v)
    {
        if (!v)
        {
            Log::error("Cannot send messages using (", protocol.typeToString(protocol.getType()), ")");
        }
        return v;
    };

    switch (protocol.getType())
    {
        case Protocol::Type::BOOST_TCP_SERVER:
        {
            return check(user.boostTCPSend(data));
        }
        case Protocol::Type::BOOST_UDP_SERVER:
        {
            return check(user.boostUDPSend(data));
        }
        case Protocol::Type::AF_UNIX_SOCK_STREAM_SERVER:
        {
            return check(user.unixStreamSend(data));
        }

        case Protocol::Type::BOOST_TCP_CLIENT:
        case Protocol::Type::BOOST_UDP_CLIENT:
        case Protocol::Type::AF_INET_TCP_CLIENT:
        case Protocol::Type::AF_INET_UDP_CLIENT:
        case Protocol::Type::AF_UNIX_SOCK_DGRAM_CLIENT:
        case Protocol::Type::AF_UNIX_SOCK_STREAM_CLIENT:
            Log::error("This function cannot be called with client protocol");
            return false;

        default:
        {
            Log::error("Cannot send messages using (", protocol.typeToString(protocol.getType()), ")");
            return false;
        }
    }
}

nx_data Command::createRoomCommand(uint64_t roomId, User& user, const nx_data& messageData, const std::map<std::string, boost::json::value>& params, uint64_t messageId)
{
    if (messageData.size() < 3)
    {
        Log::error("createRoomCommand: Insufficient messageData");
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

bool Command::sendRoomCommand(const nx_data& data, User& user, Protocol& protocol)
{
    auto& rooms = RoomStorage::getAllRooms();
    for (auto& room : rooms)
    {
        if (room.getId() == user.getRoomId())
        {
            for (auto& roomClient : room.getClients())
            {
                auto* client = ClientStorage::getClientById(roomClient);
                return sendMessageToClient(data, *client, protocol);
            }
        }
    }
    return false;
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

nx_data Command::clientMessageData(CommandType cmd, const std::string& type, uint64_t message_id, const ClientMsgType& params)
{
    auto vector = Util::convertToByteVector(Json::createJSON(clientMessageMap(cmd, type, message_id, params)));
    vector.emplace_back('\n');
    return vector;
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
