#include "nexilis/command_type.hh"
#include "nexilis/protocol.hh"
#include <cstdint>
#include <nexilis/command.hh>
#include <nexilis/dispatcher.hh>
#include <nexilis/log.hh>

#include <sys/types.h>

namespace nexilis
{

Authentication* Command::m_authentication = nullptr;

std::vector<uint8_t> Command::create(uint8_t mainCommand, uint8_t subCommand)
{
    return create(static_cast<MainCommand>(mainCommand), subCommand);
}

// Think about this API.
std::vector<uint8_t> Command::create(MainCommand maincommand, uint8_t subCommand)
{
    switch (maincommand)
    {
        case MainCommand::ping:
        {
            return std::vector<uint8_t>{
                static_cast<uint8_t>(maincommand), static_cast<uint8_t>(subCommand)};
        }
        case MainCommand::update:
            return std::vector<uint8_t>{};
        default:
        {
            uint8_t firstByte = static_cast<uint8_t>(maincommand);
            Log::error("Something went wrong, first byte: ", firstByte);
        }
    }

    return std::vector<unsigned char>{};
}

bool Command::read(const std::vector<uint8_t>& command, Client& client, Protocol& protocol, bool readByServer)
{
    if (readByServer)
    {
        return readServer(command, client, protocol);
    }
    else
    {
        return readClient(command, client, protocol);
    }
}

bool Command::read(const char* command_data, size_t lenght, Client& client, Protocol& protocol, bool readByServer)
{
    if (readByServer)
    {
        return readServer(command_data, lenght, client, protocol);
    }
    else
    {
        return readClient(command_data, lenght, client, protocol);
    }
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

bool Command::readClient(const char* command_data, size_t lenght, Client& client, Protocol& protocol)
{
    return readClient(createVectorFromCommandPtr(command_data, lenght), client, protocol);
}

bool Command::readClient(const std::vector<uint8_t>& command, Client& client, Protocol& protocol)
{
    for (uint8_t commandByte : command)
    {
        Log::debug("Commandbyte hex: ", std::hex, static_cast<int>(commandByte));
        Log::debug("");
        Log::debug("Commandbyte char: ", static_cast<char>(commandByte));
    }
    return true;
}

bool Command::readServer(const std::vector<uint8_t>& command, Client& client, Protocol& protocol)
{
    for (uint8_t commandByte : command)
    {
        Log::debug("Commandbyte hex: ", std::hex, static_cast<int>(commandByte));
        Log::debug("");
        Log::debug("Commandbyte char: ", static_cast<char>(commandByte));
    }

    switch (static_cast<MainCommand>(command.front()))
    {
        case MainCommand::protocol_setup:
        {
            switch (command[1])
            {
                // UDP setup for the client.
                case 0x10:
                {
                    auto payload = createVectorWithoutHeaderBytes(command);
                    auto port = convertToUnsignedShort(payload);
                    client.setUdpPort(port);
                    assert(port == client.getUdpPort());
                    return true;
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

        // this should be renamed connection management.
        case MainCommand::ping:
        {
            switch (command[1])
            {
                case 0x10:
                {
                    // Consider doing this check with the third byte.
                    auto type = protocol.getType();

                    // DEBUG default ping port
                    //Dispatcher::sendUDPMessage(connection, 54209, "pong");

                    switch (type)
                    {
                        case Protocol::Type::UDP:
                        {
                            // Send back to second byte.
                            uint8_t data[] = { 0x20, 0x20 };
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
                    Dispatcher::sendUDPMessage(client, client.getUdpPort(), "pong");
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

                // Check authentication.
                case 0x20:
                {
                    auto payload = createVectorWithoutHeaderBytes(command);
                    std::string password = convertToString(payload);

                    if (m_authentication)
                    {
                        if (m_authentication->checkPassword(password))
                        {
                            client.setAccess(true);
                            Log::info("Client ", client.getIPAddress(), " has access!");
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
                    return true;
                }
            }
        }

        default: return false;
    }

    return false;
}

bool Command::readServer(const char* command_data, size_t lenght, Client& connection, Protocol& protocol)
{
    return readServer(createVectorFromCommandPtr(command_data, lenght), connection, protocol);
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
