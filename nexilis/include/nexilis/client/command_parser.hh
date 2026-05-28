#ifndef NEXILIS_CLIENT_COMMAND_PARSER_HH
#define NEXILIS_CLIENT_COMMAND_PARSER_HH

#include <nexilis/client/base_api_command.hh>
#include <nexilis/client/command/error.hh>
#include <nexilis/command_type.hh>

namespace nexilis::client
{

class CommandParser
{
public:
    static std::unique_ptr<BaseAPICommand> parse(const boost::json::object& json);

private:
    static std::unique_ptr<BaseAPICommand> parseSettingCommand(const boost::json::object& json, std::string_view type);
    static std::unique_ptr<BaseAPICommand> parseGettingCommand(const boost::json::object& json, std::string_view type);
    static std::unique_ptr<BaseAPICommand> parseRoomCommand(const boost::json::object& json, std::string_view type);
    static std::unique_ptr<BaseAPICommand> parseErrorCommand(std::string_view type);
};

} // namespace nexilis::client

#endif
