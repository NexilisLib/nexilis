#include "nexilis/af_unix/sock_dgram/unix_socket_sender.hh"
#include <cstdint>
#include <nexilis/client_storage.hh>
#include <nexilis/command_type.hh>
#include <nexilis/protocol.hh>
#include <nexilis/command.hh>
#include <nexilis/dispatcher.hh>
#include <nexilis/log.hh>

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
                // I don't actually like this, should be changed.
                case 0x10:
                {
                    switch (command[2])
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
                            auto payload = removeAmountOfBytesFromVector(command, 3);
                            std::string username = convertToString(payload);
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

                        // Password to the client?
                    }

                    default: return false;
                }

                // Same for the websocket.
                case 0x20:
                {
                    return false;
                }

            }
            Log::error("Undefined control flow");
            return false;
        }

        case MainCommand::get:
        {
            switch (command[1])
            {
                // Get client id.
                case 0x10:
                {
                    // Reminder of the client parsing.
                    // 0x20 = GET
                    // 0x10 = IP
                    std::vector<uint8_t> data = { 0x20, 0x10 };

                    auto idBytes = Util::convertToByteVector(client.getId());

                    for (uint8_t i = 0; i < idBytes.size(); i++)
                    {
                        data.push_back(idBytes[i]);
                    }

                    sendMessageToClient(data);
                    return true;
                }
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
                    // DEBUG default ping port
                    //Dispatcher::sendUDPMessage(connection, 54209, "pong");

                    switch (protocol.getType())
                    {
                        case Protocol::Type::UDP:
                        {
                            // Send back to second byte.
                            Log::debug("PING Sending UDP port ", client.getUdpPort(), " back pong");
                            Dispatcher::sendUDPMessage(client, client.getUdpPort(), "pong");
                            return true;
                        }

                        case Protocol::Type::Websocket:
                        {
                            // Dispatcher::sendMessage();
                            Log::error("Websocket 0x10 not implemented");
                            return false;
                        }

                        case Protocol::Type::UnixSocket:
                        {
                            Log::error("Unix socket 0x10 not implemented");
                            return false;
                        }
                    }

                    // This branch should not exist.
                    Log::error("Received MainCommand::ping(0x10, 0x10)");
                    return false;
                }

                // 0x20, 0x20 is the reply to the pong message back.
                // Consider this in the clientside api.
                case 0x20:
                {
                    Log::info("Received pong");
                    //Dispatcher::sendUDPMessage(client, client.getUdpPort(), "pong");
                    return true;
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
                // Option for general info.
                case 0x10:
                {
                    break;
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
                    auto payload = removeAmountOfBytesFromVector(command, 2);
                    std::string password = convertToString(payload);

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
                    auto payload = removeAmountOfBytesFromVector(command, 2);
                    std::string password = convertToString(payload);

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

                // We could implement more authentication methods here.
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
                            auto payload = removeAmountOfBytesFromVector(command, 3);
                            std::string chat = convertToString(payload);

                            Log::debug("Chat: ", chat);

                            auto& allClients = ClientStorage::getAllClients();
                            Log::info("New client amount: ", allClients.size());

                            /*
                            switch (protocol.getType())
                            {
                                case Protocol::Type::UDP:
                                {
                                    for (auto&& c : allClients)
                                    {
                                        Log::debug("SENDING UDP PORT", c.getUdpPort());
                                        Dispatcher::sendUDPMessage(c, c.getUdpPort(), chat);
                                    }
                                    return true;
                                }
                                case Protocol::Type::Websocket:
                                {
                                    return false;
                                }
                                case Protocol::Type::UnixSocket:
                                {
                                    return false;
                                }
                            }
                            */
                        }

                        default: return false;
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

        default: return false;
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

unsigned short Command::convertToUnsignedShort(const std::vector<uint8_t>& bytes)
{
    if (bytes.size() < sizeof(unsigned short))
    {
        Log::error("Port conversion failed");
    }

    std::stringstream ss;
    for (uint8_t val : bytes)
    {
        ss << static_cast<char>(val);
    }
    return static_cast<unsigned short>(std::stoul(ss.str()));
}

std::string Command::convertToString(const std::vector<uint8_t>& bytes)
{
    std::string result;
    for (uint8_t b : bytes)
    {
        result += static_cast<char>(b);
    }
    return result;
}

std::vector<uint8_t> Command::removeAmountOfBytesFromVector(const std::vector<uint8_t>& original, uint8_t amount)
{
    // Return empty vector if the original vector has less elements than we want to remove.
    if (original.size() < amount)
    {
        for(auto i : original)
        {
            std::cout << std::hex << i;
        }

        Log::error("COMMAND ERROR: Cannot remove more bytes than existing command has.");
        return {};
    }

    return std::vector<uint8_t> (original.begin() + amount, original.end());
}

} // namespace nexilis
