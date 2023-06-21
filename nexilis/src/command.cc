#include "../include/nexilis/command.hh"

#include <iostream>

namespace nexilis
{

std::vector<unsigned char> Command::create(unsigned char mainCommand, unsigned char subCommand)
{
	return create(static_cast<MainCommand>(mainCommand), static_cast<SubCommand>(subCommand));
}

std::vector<unsigned char> Command::create(MainCommand maincommand, SubCommand subCommand)
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
    }
}


bool Command::read(std::vector<unsigned char> command)
{
    switch(static_cast<MainCommand>(command.front()))
    {
        case MainCommand::ping:
        {
            switch(static_cast<SubCommand>(command.front() + 1))
            {
                case SubCommand::option1:
                {
                    break;
                }

                case SubCommand::option2:
                {
                    break;
                }

                case SubCommand::option3:
                {
                    break;
                }
            }
            std::cout << "Server pinged!" << std::endl;
            return true;
        }
        case MainCommand::info:
        {
            break;
        }
    }
    return false;
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
