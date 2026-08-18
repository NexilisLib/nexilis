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

Command::Command(const ServerConfig& settings)
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

    // DefaultArgs from Commands.
    DefaultArgs args(user, protocol, command, messageId);

    if (!Movement::isInitialized())
    {
        Movement::_initialize(m_settings);
    }

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
                            return ServerImpl::set_general_clientId(args);
                        }

                        // Set username to the client.
                        case 1:
                        {
                            return ServerImpl::set_general_username(args);
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
                                    return ServerImpl::set_protocol_boosttcp_port(args);
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
                            return ServerImpl::get_general_clientId(args);
                        }
                        case 1:
                        {
                            return ServerImpl::get_general_roomId(args);
                        }
                    }
                    return CommandResult::not_found;
                }
                // Server info type
                case 1:
                {
                    switch (arg3)
                    {
                        case 0:
                        {
                            return ServerImpl::get_info_general(args);
                        }
                        case 1:
                        {
                            return ServerImpl::get_info_clients(args);
                        }
                        case 2:
                        {
                            return ServerImpl::get_info_rooms(args);
                        }
                    }
                    return CommandResult::not_found;
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
                            return ServerImpl::room_management_join(args);
                        }

                        // Leave current room.
                        case 1:
                        {
                            return ServerImpl::room_management_leave(args);
                        }

                        /// Create room.
                        case 2:
                        {
                            return ServerImpl::room_management_create(args);
                        }

                        /// Remove room.
                        case 3:
                        {
                            return ServerImpl::room_management_remove(args);
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
                            return ServerImpl::room_communicate_broadcast(args);
                        }

                        // othercast
                        case 1:
                        {
                            return CommandResult::unimplemented;
                            // return ServerImpl::room_communicate_othercast(args);
                        }

                        // unicast
                        case 2:
                        {
                            return CommandResult::unimplemented;
                            // return ServerImpl::room_communicate_unicast(args);
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
                            return ServerImpl::room_player2d_position(args);
                        }

                        // Dimension 2D
                        case 1:
                        {
                            return ServerImpl::room_player2d_dimension(args);
                        }

                        // Movement 2D
                        case 2:
                        {
                            return ServerImpl::room_player2d_movement(args);
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
                            return ServerImpl::room_object2d_create(args);
                        }

                        // Delete object
                        case 1:
                        {
                            return ServerImpl::room_object2d_destroy(args);
                        }

                        // Move object
                        case 2:
                        {
                            return ServerImpl::room_object2d_move(args);
                        }

                        // Create moving object
                        case 3:
                        {
                            return ServerImpl::room_object2d_createMoving(args);
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
                            return ServerImpl::room_player3d_position(args);
                        }

                        // Dimension 3D
                        case 1:
                        {
                            return ServerImpl::room_player3d_dimension(args);
                        }

                        // Movement 3D
                        case 2:
                        {
                            return ServerImpl::room_player3d_movement(args);
                        }

                        // Shoot
                        case 3:
                        {
                            return ServerImpl::room_player3d_shoot(args);
                        }
                    }
                    return CommandResult::not_found;
                }
                // Object3D
                case 5:
                {
                    switch (arg3)
                    {
                        case 0:
                        {
                            return ServerImpl::room_object3d_create(args);
                        }

                        case 1:
                        {
                            return ServerImpl::room_object3d_destroy(args);
                        }

                        case 2:
                        {
                            return ServerImpl::room_object3d_move(args);
                        }

                        case 3:
                        {
                            return ServerImpl::room_object3d_createMoving(args);
                        }
                    }
                    break;
                }

                // GameItem
                case 6:
                {
                    switch (arg3)
                    {
                        case 0:
                        {
                            return ServerImpl::room_gameitem_create(args);
                        }

                        case 1:
                        {
                            return ServerImpl::room_gameitem_update(args);
                        }

                        case 2:
                        {
                            return ServerImpl::room_gameitem_destroy(args);
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

        case CommandType::undefined:
        {
            return CommandResult::error;
        }
    }
    return CommandResult::not_found;
}

bool Command::sendMessageToClient(nx_data data, User& user, Protocol& protocol)
{
    auto trySend = [&data, &user](Protocol::Type type) -> bool
    {
        switch (type)
        {
            case Protocol::Type::BOOST_TCP_SERVER:
                return user.boostTCPSend(data);
            case Protocol::Type::BOOST_UDP_SERVER:
                return user.boostUDPSend(data);
            case Protocol::Type::AF_UNIX_SOCK_STREAM_SERVER:
                return user.unixStreamSend(data);
            default:
                return false;
        }
    };

    if (trySend(protocol.getType()))
    {
        return true;
    }

    static const Protocol::Type allServerProtocols[] = {
            Protocol::Type::BOOST_TCP_SERVER,
            Protocol::Type::BOOST_UDP_SERVER,
            Protocol::Type::AF_UNIX_SOCK_STREAM_SERVER,
    };

    for (auto fallbackType : allServerProtocols)
    {
        if (fallbackType == protocol.getType())
            continue;
        if (trySend(fallbackType))
            return true;
    }

    Log::error("Failed to send message to client via any protocol");
    return false;
}

std::string roomActionToString(RoomCommandType::Root root, uint8_t action)
{
    switch (root)
    {
        case RoomCommandType::Root::management:
            return RoomCommandType::ManagementTypeToString(
                    static_cast<RoomCommandType::Management>(action));

        case RoomCommandType::Root::player_2D:
        case RoomCommandType::Root::player_3D:
            return RoomCommandType::PlayerTypeToString(
                    static_cast<RoomCommandType::PlayerType>(action));

        case RoomCommandType::Root::object_2D:
        case RoomCommandType::Root::object_3D:
            return RoomCommandType::ObjectTypeToString(
                    static_cast<RoomCommandType::ObjectType>(action));

        case RoomCommandType::Root::game_item:
            return RoomCommandType::GameItemActionToString(
                    static_cast<RoomCommandType::GameItemAction>(action));

        case RoomCommandType::Root::communication:
            return RoomCommandType::CommunicationTypeToString(
                    static_cast<RoomCommandType::Communication>(action));

        case RoomCommandType::Root::undefined:
        default:
            Log::error("createRoomCommand: Undefined roomType or unsupported action");
            return {};
    }
}

nx_data Command::createRoomCommand(uint64_t roomId, User& user, const nx_data& messageData, const std::map<std::string, boost::json::value>& params, uint64_t messageId)
{
    if (messageData.size() < 3)
    {
        Log::error("createRoomCommand: Insufficient messageData");
        return nx_data();
    }

    auto room_type = static_cast<RoomCommandType::Root>(messageData[1]);
    auto action = messageData[2];

    std::string room_command_action = roomActionToString(room_type, action);
    if (room_command_action.empty())
    {
        Log::error("createRoomCommand: Failed to convert room action to string");
        return nx_data();
    }

    std::string room_command_type = RoomCommandType::RoomTypeToString(room_type);
    if (room_command_type.empty())
    {
        Log::error("createRoomCommand: Failed to convert roomType");
        return nx_data();
    }

    auto all_params = ClientMsgType{
            {"action", boost::json::value(room_command_action)},
            {"room_id", boost::json::value(roomId)},
            {"client_id", boost::json::value(user.getId())},
    };
    all_params.insert(params.begin(), params.end());

    return clientMessageData(CommandType::room, room_command_type, messageId, all_params);
}

bool Command::sendRoomCommand(const nx_data& data, User& user, Protocol& protocol)
{
    auto& rooms = RoomStorage::getAllRooms();
    for (auto& room : rooms)
    {
        if (room.getId() == user.getRoomId())
        {
            bool all_success = true;
            for (auto& roomClient : room.getClients())
            {
                auto* client = ClientStorage::getClientById(roomClient);
                if (!sendMessageToClient(data, *client, protocol))
                    all_success = false;
            }
            return all_success;
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
