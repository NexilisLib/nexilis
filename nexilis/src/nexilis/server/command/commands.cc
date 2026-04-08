#include <nexilis/server/command/command.hh>
#include <nexilis/server/command/commands.hh>

namespace nexilis::server
{

Commands::DefaultArgs::DefaultArgs(User& user, Protocol& protocol, const nx_data& data, size_t messageId)
    : m_user(user),
      m_protocol(protocol),
      m_data(data),
      m_messageId(messageId)
{
}

CommandResult Commands::Get::Info::create(const DefaultArgs& args, const std::pair<boost::json::object, std::string>& info_data)
{
    Command::ClientMsgType params;
    for (const auto& [key, value] : info_data.first)
    {
        params[key] = value;
    }
    auto data = Command::clientMessageData(CommandType::getting, info_data.second, args.getMessageId(), params);
    if (Command::sendMessageToClient(data, args.getUser(), args.getProtocol()))
    {
        return CommandResult::success;
    }
    else
    {
        return CommandResult::failed_room_send;
    }
}

} // namespace nexilis::server
