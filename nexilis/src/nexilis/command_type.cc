#include <nexilis/command_type.hh>

namespace nexilis
{

std::string commandTypeAsString(CommandType command_type)
{
    switch (command_type)
    {
        case CommandType::setting:
            return "setting";
        case CommandType::getting:
            return "getting";
        case CommandType::room:
            return "room";
        case CommandType::authentication:
            return "authentication";
        case CommandType::server_management:
            return "server_management";
        case CommandType::player_management:
            return "player_management";
        case CommandType::error:
            return "error";
        case CommandType::info:
            return "info";
        case CommandType::undefined:
            return "undefined";
    }
    return "undefined";
}

CommandType stringAsCommandType(const std::string& str)
{
    if (str == "setting")
        return CommandType::setting;
    else if (str == "getting")
        return CommandType::getting;
    else if (str == "room")
        return CommandType::room;
    else if (str == "authentication")
        return CommandType::authentication;
    else if (str == "server_management")
        return CommandType::server_management;
    else if (str == "player_management")
        return CommandType::player_management;
    else if (str == "error")
        return CommandType::error;
    else if (str == "info")
        return CommandType::info;
    else
        return CommandType::undefined;
}

} // namespace nexilis
