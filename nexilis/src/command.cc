#include <boost/json/serialize.hpp>
#include <nexilis/command.hh>
#include <nexilis/client_storage.hh>
#include <nexilis/command_type.hh>
#include <nexilis/log.hh>
#include <nexilis/protocol.hh>
#include <nexilis/json.hh>
#include <nexilis/common/util.hh>

namespace nexilis
{

Authentication* Command::m_authentication = nullptr;

bool Command::read(const char* command_data, size_t lenght, Client& client, Protocol& protocol, const std::function<void(const std::vector<uint8_t>&)> sendMessageToClient)
{
    return Command::read(Command::createVectorFromCommandPtr(command_data, lenght), client, protocol, sendMessageToClient);
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

bool Command::read(const std::vector<uint8_t>& command, Client& client, Protocol& protocol, const std::function<void(const std::vector<uint8_t>&)> sendMessageToClient)
{
    for (uint8_t commandByte : command)
    {
        Log::debug("Commandbyte hex: ", std::hex, static_cast<int>(commandByte));
        Log::debug("");
        Log::debug("Commandbyte char: ", static_cast<char>(commandByte));
    }

    switch (static_cast<MainCommand>(command.front()))
    {
        case MainCommand::set:
        {
            switch (command[1])
            {
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
                case 0x10:
                {
                    /// Client package: 0x10, 0x10 = SET, ID.
                    std::vector<uint8_t> data = {0x10, 0x10};

                    auto idBytes = Util::convertToByteVector(client.getId());

                    for (uint8_t i = 0; i < idBytes.size(); i++)
                    {
                        data.push_back(idBytes[i]);
                    }

                    Log::info("sending to client");
                    sendMessageToClient(data);
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
                case 0x10:
                {
                    Log::debug("PING Sending UDP port ", client.getUdpPort(), " back pong");
                    // TODO create pong message, I mean this is kinda stupid.
                    std::vector<uint8_t> message = {0x10, 0x10};
                    sendMessageToClient(message);
                    return true;
                }

                case 0x20:
                {
                    return false;
                }

                // Start listening
                case 0x30:
                {
                    return false;
                }

                // Stop listening
                case 0x40:
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
                case 0x10:
                {
                    boost::json::object serverData = Json::getServerData();
                    std::string stringData = boost::json::serialize(serverData);
                    auto data = Util::convertToByteVector(stringData.c_str(), stringData.size());

                    sendMessageToClient(data);
                    return true;
                }

                // Option for help.
                case 0x20:
                {
                    break;
                }
            }
            break;
        }

        case MainCommand::authentication:
        {
            switch (command[1])
            {
                // Setup authentication.
                case 0x10:
                {
                    Log::info("New client wants to authenticate, not implemented");
                    return false;
                }

                // Check authentication for root access.
                case 0x20:
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
                case 0x30:
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
                case 0x10:
                {
                    switch (command[2])
                    {
                        // String message.
                        case 0x10:
                        {
                            auto payload = Util::removeAmountOfBytesFromVector(command, 3);
                            std::string chat = Util::convertToString(payload);

                            Log::debug("Chat: ", chat);

                            auto& allClients = ClientStorage::getAllClients();
                            Log::info("New client amount: ", allClients.size());

                            switch (protocol.getType())
                            {
                                default:
                                    return false;
                            }
                        }

                        default:
                            return false;
                    }
                }

                // Client sends a message to everyone except itself.
                case 0x20:
                {
                    switch (command[2])
                    {
                        // String message.
                        case 0x10:
                        {
                        }
                    }
                }

                // Client sends a message in specific context.
                case 0x30:
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
                case 0x10:
                {
                    Log::critical("Internal server error: x");
                    return true;
                }
                default:
                    return false;
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

} // namespace nexilis
