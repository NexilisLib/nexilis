#include <nexilis/server/client_storage.hh>
#include <nexilis/server/command/command.hh>
#include <nexilis/server/command/command_result.hh>
#include <nexilis/server/command/commands.hh>

namespace nexilis::server
{

CommandResult Commands::Set::Protocol::BoostTCP::port(const DefaultArgs& args)
{
    auto payload = Util::removeAmountOfBytesFromVector(args.getData(), 4);
    uint16_t port = Util::convertoToUint16(payload);

    auto data = Command::clientMessageData(CommandType::setting, "port", args.getMessageId(), {{"boost_tcp_port", boost::json::value(port)}});
    Command::sendMessageToClient(args.getData(), args.getUser(), args.getProtocol());
    return CommandResult::success;
}

} // namespace nexilis::server
