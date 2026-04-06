#include <nexilis/server/command/command.hh>
#include <nexilis/server/command/commands.hh>
#include <nexilis/server/room_storage.hh>

namespace nexilis::server
{

CommandResult Commands::Room::Object2D::move(const DefaultArgs& args)
{
    auto& user = args.getUser();

    // Get messagedata.
    auto payload = Util::removeAmountOfBytesFromVector(args.getData(), 3);
    auto objectId = Util::uint64FromFront(payload);
    auto position = Util::vector2fFromFront(Util::removeAmountOfBytesFromVector(payload, 8));

    // Get object from server storage.
    auto room = RoomStorage::getRoomById(user.getRoomId());
    if (!room)
    {
        Log::error("Client room not found!");
        return CommandResult::failure;
    }

    auto object = room->getObject2DById(objectId);
    if (!object)
    {
        Log::error("Object not found with id: ", objectId);
        return CommandResult::failure;
    }

    auto oldPosition = object->getPosition();
    auto newPosition = oldPosition + position;

    std::map<std::string, boost::json::value> params{
            {"objectId", boost::json::value(objectId)},
            {"x", boost::json::value(newPosition.x)},
            {"y", boost::json::value(newPosition.y)}};

    // Move object in server storage.
    object->setPosition(newPosition);

    auto roomCommand = Command::createRoomCommand(user.getRoomId(), user, args.getData(), params, args.getMessageId());
    if (Command::sendRoomCommand(roomCommand, user, args.getProtocol()))
    {
        return CommandResult::success;
    }
    else
    {
        return CommandResult::failed_room_send;
    }
}

} // namespace nexilis::server
