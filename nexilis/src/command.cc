#include <nexilis/command.hh>
#include <nexilis/dispatcher.hh>
#include <nexilis/log.hh>

namespace nexilis
{

std::vector<unsigned char> Command::create(unsigned char mainCommand, unsigned char subCommand)
{
    return create(static_cast<MainCommand>(mainCommand), subCommand);
}

std::vector<unsigned char> Command::create(MainCommand maincommand, unsigned char subCommand)
{
    switch (maincommand)
    {
        case MainCommand::ping:
        {
            return std::vector<unsigned char>{
                static_cast<unsigned char>(maincommand), static_cast<unsigned char>(subCommand)};
        }
        case MainCommand::setup:
            return std::vector<unsigned char>{};
        case MainCommand::update:
            return std::vector<unsigned char>{};
        default:
        {
            unsigned char firstByte = static_cast<unsigned char>(maincommand);
            Log::error("Something went wrong, first byte: ", firstByte);
        }
    }

    return std::vector<unsigned char>{};
}

bool Command::read(const std::vector<unsigned char>& command, Connection& connection, Protocol& protocol)
{
    switch (static_cast<MainCommand>(command.front()))
    {
        case MainCommand::ping:
        {
            switch (command[1])
            {
                case 0x10:
                {
                    auto type = protocol.getType();

                    switch (type)
                    {
                        case Protocol::Type::UDP:
                        {
                            Dispatcher::sendUDPMessage(connection, 54209, "pong");
                            break;
                        }

                        case Protocol::Type::Websocket:
                        {
                            // Dispatcher::sendMessage();
                            break;
                        }
                    }

                    Log::info("Received MainCommand::ping(0x10, 0x10)");
                    return true;
                }

                case 0x20:
                {
                }

                default:
                {
                    return true;
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
    }

    return false;
}

bool Command::read(const char* command_data, size_t lenght, Connection& connection, Protocol& protocol)
{
    // Create a vector and reserve space for the character.
    std::vector<unsigned char> result;
    result.reserve(lenght);

    for (size_t i = 0; i < lenght; i++)
    {
        result.emplace_back(static_cast<unsigned char>(command_data[i]));
    }
    return read(result, connection, protocol);
}

std::string Command::createIPv4Address(const std::vector<unsigned char>& characters)
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
