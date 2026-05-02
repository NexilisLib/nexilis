#include <nexilis/server/command/command.hh>
#include <nexilis/server/command/commands.hh>

namespace nexilis::server
{

CommandResult ServerImpl::get_general_roomId(const DefaultArgs& args)
{
    Log::debug("getting::general::client_id", args);

    std::map<std::string, boost::json::value> params{
            {"client_id", boost::json::value(args.getUser().getId())}};

    auto data = Command::clientMessageData(CommandType::getting, "room_id", args.getMessageId(), params);
    Command::sendMessageToClient(data, args.getUser(), args.getProtocol());

    return CommandResult::success;
}
} // namespace nexilis::server
