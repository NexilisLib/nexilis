#include <nexilis/server/command/command.hh>
#include <nexilis/server/command/commands.hh>
#include <nexilis/server/room_storage.hh>

namespace nexilis::server
{

CommandResult ServerImpl::room_communicate_broadcast(const DefaultArgs& args)
{
    auto& user = args.getUser();
    auto payload = Util::removeAmountOfBytesFromVector(args.getData(), 3);
    auto messageData = Util::convertToString(payload);

    if (user.getRoomId() == 0)
    {
        Log::error("User not currently in room!");
        return CommandResult::error;
    }

    auto* room = RoomStorage::getRoomById(user.getRoomId());
    if (!room)
    {
        Log::error("Cannot find room for broadcast!");
        return CommandResult::error;
    }
    room->addBroadcast(user.getId(), messageData);

    std::map<std::string, boost::json::value> params{
            {"id", boost::json::value(user.getId())},
            {"room_id", boost::json::value(user.getRoomId())},
            {"message", boost::json::value(messageData)}};

    auto roomCommand = Command::createRoomCommand(user.getRoomId(), user, args.getData(), params, args.getMessageId());
    Command::sendRoomCommand(roomCommand, user, args.getProtocol());
    return CommandResult::success;
}
} // namespace nexilis::server
