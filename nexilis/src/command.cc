#include "../include/nexilis/command.hh"
#include "../include/nexilis/dispatcher.hh"

#include <iostream>

namespace nexilis
{

std::vector<unsigned char> Command::create(unsigned char mainCommand, unsigned char subCommand)
{
    return create(static_cast<MainCommand>(mainCommand), subCommand);
}

std::vector<unsigned char> Command::create(MainCommand maincommand, unsigned char subCommand)
{
    switch(maincommand)
    {
        case MainCommand::ping:
        {
            return std::vector<unsigned char>
            {
                static_cast<unsigned char>(maincommand), static_cast<unsigned char>(subCommand)
            };
        }
        case MainCommand::setup: return std::vector<unsigned char> {};
        case MainCommand::update: return std::vector<unsigned char> {};
        default:
        {
            std::cout << "something went wrong" << std::endl;
            unsigned char firstByte = static_cast<unsigned char>(maincommand);
            std::cout << "first byte: " << firstByte << std::endl;
            std::cout << "second byte: " << subCommand << std::endl;
        }
    }

    return std::vector<unsigned char> {};
}

bool Command::read(const std::vector<unsigned char>& command, Connection& connection)
{
    switch(static_cast<MainCommand>(command.front()))
    {
        case MainCommand::ping:
        {
            std::cout << "ping called" << std::endl;
            //Dispatcher::sendWebsocketMessage(connection, "Hey from server!");
            Dispatcher::sendUDPMessage(connection, "hey from udp");
            return true;
        }
        case MainCommand::info:
        {
            switch(command[1])
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

bool Command::read(const char* command_data, size_t lenght, Connection& connection)
{
    // Create a vector and reserve space for the character.
    std::vector<unsigned char> result;
    result.reserve(lenght);

    for(size_t i = 0; i < lenght; i++)
    {
        result.emplace_back(static_cast<unsigned char>(command_data[i]));
    }
    return read(result, connection);
}

std::string Command::createIPv4Address(const std::vector<unsigned char>& characters)
{
    if (characters.size() < 4)
    {
        std::cerr << "Insufficient characters to create an IPv4 address." << std::endl;
        return "";
    }

    std::string ipAddress;
    ipAddress += std::to_string(characters[0]) + "." +
                 std::to_string(characters[1]) + "." +
                 std::to_string(characters[2]) + "." +
                 std::to_string(characters[3]);

    return ipAddress;
}

}
