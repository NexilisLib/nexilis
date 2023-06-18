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
                static_cast<unsigned char>(maincommand), 2
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


}
