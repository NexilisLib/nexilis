#include <nexilis/command.hh>
#include <nexilis/command_type.hh>
#include <nexilis/common/util.hh>
#include <nexilis/json.hh>
#include <nexilis/log.hh>
#include <nexilis/protocol.hh>
#include <nexilis/client_storage.hh>
#include <nexilis/room_storage.hh>
#include <boost/json/serialize.hpp>

namespace nexilis
{

Authentication* Command::m_authentication = nullptr;

bool Command::read(const char* command_data, size_t lenght, Client& client, Protocol& protocol)
{
    return Command::read(Command::createVectorFromCommandPtr(command_data, lenght), client, protocol);
}

std::vector<uint8_t> Command::createVectorFromCommandPtr(const char* command_data, size_t lenght)
{
    std::vector<uint8_t> result;
    result.reserve(lenght);

    for (size_t i = 0; i < lenght; i++)
    {
        result.emplace_back(static_cast<uint8_t>(command_data[i]));
    }
    return result;
}

bool Command::read(const std::vector<uint8_t>& command, Client& client, Protocol& protocol)
{
    Log::debug("Command: Nexilis command sequence");
    for (uint8_t commandByte : command)
    {
        Log::debug("Commandbyte hex: ", std::hex, static_cast<int>(commandByte));
        Log::debug("Commandbyte char: ", static_cast<char>(commandByte));
    }

    switch (static_cast<MainCommand>(command.front()))
    {
        case MainCommand::set:
        {
            switch (command[1])
            {
                // This makes no sense so let's put something else here.
                /*
                case 0x10:
                {
                    auto port = Util::uint8PairToUint16(command[3], command[4]);
                    auto& clients = ClientStorage::getAllClients();

                    for (auto c = clients.begin(); c != clients.end(); c++)
                    {
                        if (*c == client)
                        {
                            c->setUdpPort(port);
                        }
                    }
                    return true;
                }
                */

                // Give username to the client.
                case 0x20:
                {
                    auto payload = Util::removeAmountOfBytesFromVector(command, 3);
                    std::string username = Util::convertToString(payload);
                    auto& clients = ClientStorage::getAllClients();

                    for (auto c = clients.begin(); c != clients.end(); c++)
                    {
                        if (*c == client)
                        {
                            c->setUsername(username);
                        }
                    }

                    return true;
                }

                default:
                    return false;
            }
            return false;
        }

        case MainCommand::get:
        {
            switch (command[1])
            {
                // Get client id.
                case 0:
                {
                    Log::info("Client id before send: ", client.getId());
                    // Fix this, nexilis_status = 1 is correct tho.
                    std::map<std::string, boost::json::value> data{
                            {"nexilis_status", boost::json::value(1)},
                            { "type", boost::json::value("set_client_id")},
                            {"set_client_id", boost::json::value(client.getId())}};
                    auto json = Json::createJSON(data);
                    std::vector<uint8_t> message = Util::convertToByteVector(json);
                    sendMessageToClient(message, client, protocol);
                    Log::info("sent message to client");
                    return true;
                }
                default:
                    return false;
            }
        }

        // this should be renamed connection management.
        case MainCommand::ping:
        {
            switch (command[1])
            {
                case 0:
                {
                    // TODO create pong message, I mean this is kinda stupid.
                    return false;
                }

                case 1:
                {
                    return false;
                }

                // Start listening
                case 2:
                {
                    return false;
                }

                // Stop listening
                case 3:
                {
                    return false;
                }

                default:
                {
                    return false;
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

                    sendMessageToClient(data, client, protocol);
                    Log::info("Used Info::generalInfo");
                    return true;
                }

                // Get data from the clients existing on the server.
                case 1:
                {
                    auto message = Json::getNexilisStatus(2);
                    Json::emplace(message, Json::getClientDataMessage());
                    std::string stringData = boost::json::serialize(message);
                    auto data = Util::convertToByteVector(stringData.c_str(), stringData.size());

                    sendMessageToClient(data, client, protocol);
                    Log::info("Used Info::clientInfo");
                    return true;
                }

                // Get data from the rooms existing on the server.
                case 2:
                {
                    auto message = Json::getNexilisStatus(2);
                    Json::emplace(message, Json::getRoomDataMessage());
                    std::string stringData = boost::json::serialize(message);
                    auto data = Util::convertToByteVector(stringData.c_str(), stringData.size());

                    sendMessageToClient(data, client, protocol);
                    Log::info("Used Info::roomInfo");
                    return true;
                }
                default: return false;
            }
            return false;
        }

        case MainCommand::authentication:
        {
            switch (command[1])
            {
                // Setup authentication.
                case 0:
                {
                    Log::info("New client wants to authenticate, not implemented");
                    return false;
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
                            client.setRootAccess(true);
                            Log::info("Client ", client.getIPAddress(), " has root access!");
                            return true;
                        }
                        else
                        {
                            Log::error("Wrong password!");
                        }
                    }
                    else
                    {
                        Log::error("Trying to set password for server without auth!");
                    }
                    return false;
                }

                // Set authentication for valid client.
                case 2:
                {
                    auto payload = Util::removeAmountOfBytesFromVector(command, 2);
                    std::string password = Util::convertToString(payload);

                    if (m_authentication)
                    {
                        if (m_authentication->isCommonPassword(password))
                        {
                            client.setCommonAccess(true);
                            Log::info("Client ", client.getIPAddress(), " has common access!");
                            return true;
                        }
                        else
                        {
                            Log::error("Wrong password!");
                            return false;
                        }
                    }
                    else
                    {
                        Log::error("Trying to set password for server without auth!");
                        return false;
                    }

                    return false;
                }

                default:
                    return false;
            }
        }

        case MainCommand::server_management:
        {
            return false;
        }

        case MainCommand::player_management:
        {
            return false;
        }

        case MainCommand::communicate:
        {
            switch (command[1])
            {
                // Client sends message to everyone.
                case 0:
                {
                    switch (command[2])
                    {
                        // Default state.
                        case 0:
                        {
                            auto payload = Util::removeAmountOfBytesFromVector(command, 3);

                            std::map<std::string, boost::json::value> data{
                                    {"nexilis_status", boost::json::value(1)},
                                    {"type", boost::json::value("broadcast")},
                                    {"message", boost::json::value(Util::convertToString(payload))}};

                            auto json = Json::createJSON(data);
                            std::vector<uint8_t> message = Util::convertToByteVector(json);

                            auto& clients = ClientStorage::getAllClients();
                            for (auto& c : clients)
                            {
                                sendMessageToClient(message, c, protocol);
                            }
                            return true;
                        }
                        default:
                            return false;
                    }
                }

                // Client sends a message to everyone except itself.
                case 1:
                {
                    switch (command[2])
                    {
                        // Default state.
                        case 0:
                        {
                            auto payload = Util::removeAmountOfBytesFromVector(command, 3);
                            auto& clients = ClientStorage::getAllClients();

                            for (auto& c : clients)
                            {
                                if (c.getId() != client.getId())
                                {
                                    sendMessageToClient(payload, c, protocol);
                                }
                            }
                            return true;
                        }
                        default:
                            return false;
                    }
                }

                // Client sends a message in a room context.
                case 2:
                {
                }
            }
            return false;
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
                    return true;
                }
                default:
                    return false;
            }
        }

        case MainCommand::room:
        {
            switch (command[1])
            {
                // Join room x.
                case 0:
                {
                    auto payload = Util::removeAmountOfBytesFromVector(command, 2);
                    uint64_t roomId = Util::convertToType<uint64_t>(payload);

                    auto room = RoomStorage::getRoomById(roomId);

                    if (!room)
                    {
                        Log::error("Cannot find room with specified id!");
                        return false;
                    }
                    room->joinRoom(client.getId());
                    return true;
                }
                // Leave current room.
                case 1:
                {
                    return false;
                }
                default: return false;
            }
        }

        default:
            return false;
    }

    return false;
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

void Command::sendMessageToClient(std::vector<uint8_t> data, Client& client, Protocol& protocol)
{
    switch (protocol.getType())
    {
        case Protocol::Type::BOOST_TCP_SERVER:
        {
            if (!client.boostTCPSend(data))
            {
                Log::error("Cannot send messages using this protocol (BOOST_TCP)");
            }
            return;
        }

        case Protocol::Type::BOOST_UDP_SERVER:
        {
            if (!client.boostUDPSend(data))
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
