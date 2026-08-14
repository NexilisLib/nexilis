#include <nexilis/server/client_storage.hh>
#include <nexilis/server/command/command.hh>
#include <nexilis/server/command/command_result.hh>
#include <nexilis/server/command/commands.hh>

namespace nexilis::server
{

CommandResult ServerImpl::set_general_username(const DefaultArgs& args)
{
    Log::debug("setting::general::username", args);
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

    auto data = Command::clientMessageData(CommandType::setting, "username", args.getMessageId(),
                                           {{"client_id", boost::json::value(args.getUser().getId())},
                                            {"username", boost::json::value(username)}});

    // Notify the room so peers can display this client's username.
    if (client->getRoomId() != 0)
    {
        Command::sendRoomCommand(data, args.getUser(), args.getProtocol());
    }
    else
    {
        Command::sendMessageToClient(data, args.getUser(), args.getProtocol());
    }

    return CommandResult::success;
}

} // namespace nexilis::server
