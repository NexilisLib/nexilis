#include <nexilis/convert_to_map.hh>
#include <nexilis/server/command/command.hh>
#include <nexilis/server/command/commands.hh>
#include <nexilis/server/server_json.hh>

namespace nexilis::server
{

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
