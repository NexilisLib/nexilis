#include <nexilis/server/client_storage.hh>
#include <nexilis/server/command/command.hh>
#include <nexilis/server/command/command_result.hh>
#include <nexilis/server/command/commands.hh>

namespace nexilis::server
{

CommandResult Commands::Set::General::username(DefaultArgs args)
{
    Log::debug("setting::general::username");
    auto payload = Util::removeAmountOfBytesFromVector(args.getData(), 3);
    std::string username = Util::convertToString(payload);

    // Setting the username for internal client.
    auto* client = ClientStorage::getClientById(args.getUser().getId());
    if (client)
    {
        client->setUsername(username);
    }
    else
    {
        return CommandResult::error;
    }

    auto data = Command::clientMessageData(CommandType::setting, "username", args.getMessageId(), {{"username", boost::json::value(username)}});
    Command::sendMessageToClient(data, args.getUser(), args.getProtocol());

    return CommandResult::success;
}

} // namespace nexilis::server
