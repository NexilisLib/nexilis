#ifndef NEXILIS_CLIENT_COMMAND_PARSER_HH
#define NEXILIS_CLIENT_COMMAND_PARSER_HH

#include <nexilis/client/base_api_command.hh>
#include <nexilis/command_type.hh>

namespace nexilis::client
{

class CommandParser
{
public:
    static std::unique_ptr<BaseAPICommand> parse(const boost::json::object& json)
    {
        if (!json.contains("command") || !json.contains("type"))
        {
            // return std::make_unique<ErrorCommand>(ReadResult::error);
            return nullptr;
        }

        auto cmd_type = commandTypeFromString(json.at("command").as_string().c_str());
        auto type = json.at("type").as_string();

        switch (cmd_type)
        {
            case CommandType::setting:
                return parseSettingCommand(json, type);
            case CommandType::getting:
            case CommandType::authentication:
            case CommandType::room:
            case CommandType::error:
            default:
                return nullptr;
        }
        return nullptr;
    }

private:
    static std::unique_ptr<BaseAPICommand> parseSettingCommand(const boost::json::object& json, std::string_view type);
};

} // namespace nexilis::client

#endif
