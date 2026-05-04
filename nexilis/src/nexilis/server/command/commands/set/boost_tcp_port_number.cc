#include <nexilis/server/client_storage.hh>
#include <nexilis/server/command/command.hh>
#include <nexilis/server/command/command_result.hh>
#include <nexilis/server/command/commands.hh>

namespace nexilis::server
{

CommandResult ServerImpl::set_protocol_boosttcp_port(const DefaultArgs& args)
{
    Log::info("Commands::Set::Protocol::BoostTCP::port used!");
    auto payload = Util::removeAmountOfBytesFromVector(args.getData(), 4);
    uint16_t port = Util::convertoToUint16(payload);

    auto data = Command::clientMessageData(CommandType::setting, "port", args.getMessageId(), {{"boost_tcp_port", boost::json::value(port)}});
    if (Command::sendMessageToClient(data, args.getUser(), args.getProtocol()))
    {
        return CommandResult::success;
    }
    return CommandResult::failed_room_send;
}

} // namespace nexilis::server
