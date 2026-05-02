#include <nexilis/server/command/command.hh>
#include <nexilis/server/command/commands.hh>
#include <nexilis/server/room_storage.hh>

namespace nexilis::server
{

CommandResult ServerImpl::room_player2d_dimension(const DefaultArgs& args)
{
    auto& user = args.getUser();
    auto payload = Util::removeAmountOfBytesFromVector(args.getData(), 3);
    auto vector = Util::convertToVector2(payload);
    Log::debug("Dimension x:", vector.x, " y:", vector.y);

    auto currentRoom = RoomStorage::getRoomById(user.getRoomId());
    if (!currentRoom)
    {
        Log::error("Client not currently in room.");
        return CommandResult::failure;
    }

    std::map<std::string, boost::json::value> params{
            {"x", boost::json::value(vector.x)},
            {"y", boost::json::value(vector.y)}};

    user.getObject2D().setDimensions(vector);

    auto roomCommand = Command::createRoomCommand(user.getRoomId(), user, args.getData(), params, args.getMessageId());
    if (Command::sendRoomCommand(roomCommand, user, args.getProtocol()))
    {
        return CommandResult::success;
    }
    return CommandResult::failure;
}

} // namespace nexilis::server
