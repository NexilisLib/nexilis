#include <nexilis/convert_to_map.hh>
#include <nexilis/server/command/command.hh>
#include <nexilis/server/command/commands.hh>
#include <nexilis/server/server_json.hh>

namespace nexilis::server
{

void merge_boost_json(const boost::json::object& from, boost::json::object& to)
{
    for (const auto& [key, value] : from)
    {
        if (!to.contains(key))
        {
            to[key] = value;
        }
    }
}

CommandResult ServerImpl::get_info_general(const DefaultArgs& args)
{
    Log::debug("get_info_general: ", args);

    auto room_data = ServerJson::getRoomData();
    auto client_data = ServerJson::getClientData();
    merge_boost_json(client_data, room_data);
    auto params = convert_to_map(room_data);
    auto data = Command::clientMessageData(CommandType::getting, "info_general", args.getMessageId(), params);

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
