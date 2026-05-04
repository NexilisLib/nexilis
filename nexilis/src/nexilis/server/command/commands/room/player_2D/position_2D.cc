#include <nexilis/server/command/commands.hh>

#include <nexilis/server/command/command.hh>
#include <nexilis/server/room_storage.hh>

namespace nexilis::server
{

CommandResult ServerImpl::room_player2d_position(const DefaultArgs& args)
{
    auto& user = args.getUser();

    if (user.getRoomId() == 0)
    {
        Log::error("User not in room!");
        return CommandResult::error;
    }

    auto payload = Util::removeAmountOfBytesFromVector(args.getData(), 3);
    auto vector = Util::convertToVector2(payload);
    Log::debug("Position x:", vector.x, " y:", vector.y);

    auto currentRoom = RoomStorage::getRoomById(user.getRoomId());

    if (!currentRoom)
    {
        Log::warning("Client not currently in room.");
        return CommandResult::failure;
    }

    user.getObject2D().setPosition(vector);

    std::map<std::string, boost::json::value> params{
            {"x", boost::json::value(vector.x)},
            {"y", boost::json::value(vector.y)}};

    auto roomCommand = Command::createRoomCommand(user.getRoomId(), user, args.getData(), params, args.getMessageId());
    if (Command::sendRoomCommand(roomCommand, user, args.getProtocol()))
    {
        return CommandResult::success;
    }
    else
    {
        return CommandResult::failure;
    }
}

} // namespace nexilis::server
