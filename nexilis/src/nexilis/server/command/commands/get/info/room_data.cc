#include <nexilis/server/command/command.hh>
#include <nexilis/server/command/commands.hh>
#include <nexilis/server/server_json.hh>

#include <map>

namespace nexilis::server
{

namespace json = boost::json;

// Function to convert boost::json::object to std::map
std::map<std::string, json::value> convert_to_map(const json::object& obj)
{
    std::map<std::string, json::value> result;

    // Iterate over each key-value pair in the JSON object
    for (const auto& kv : obj)
    {
        // kv.key() returns a string view, kv.value() returns a boost::json::value
        result[std::string(kv.key())] = kv.value();
    }

    return result;
}

CommandResult ServerImpl::get_info_rooms(const DefaultArgs& args)
{
    Log::debug("get_info_rooms: ", args);

    auto d = ServerJson::getRoomData();
    auto params = convert_to_map(d);
    auto data = Command::clientMessageData(CommandType::getting, "info_rooms", args.getMessageId(), params);

    if (Command::sendMessageToClient(data, args.getUser(), args.getProtocol()))
    {
        return CommandResult::success;
    }
    else
    {
        return CommandResult::error;
    }
}

} // namespace nexilis::server
