#include <nexilis/convert_to_map.hh>
#include <nexilis/server/command/command.hh>
#include <nexilis/server/command/commands.hh>
#include <nexilis/server/server_json.hh>

namespace nexilis::server
{

CommandResult ServerImpl::get_info_clients(const DefaultArgs& args)
{
    Log::debug("get_info_clients: ", args);

    auto d = ServerJson::getClientData();
    auto params = convert_to_map(d);
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
