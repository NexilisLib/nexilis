#include <nexilis/object/object_2d.hh>
#include <nexilis/server/command/command.hh>
#include <nexilis/server/command/commands.hh>
#include <nexilis/server/room_storage.hh>

namespace nexilis::server
{

CommandResult Commands::Room::Object2D::create(const DefaultArgs& args)
{
    auto& user = args.getUser();
    auto data = args.getData();
    auto payload = Util::removeAmountOfBytesFromVector(data, 3);
    auto position = Util::vector2fFromFront(payload);
    auto dimensions = Util::vector2fFromFront(Util::removeAmountOfBytesFromVector(payload, 8));
    auto fileBytes = Util::removeAmountOfBytesFromVector(payload, 16);
    auto filepath = Util::convertToString(fileBytes);

    // Create server object.
    auto object = nexilis::Object2D(Util::getRandomUint64(), position, dimensions);
    object.setFilepath(filepath);
    uint64_t objectId = object.getId();
    Log::info("Created object with id: ", objectId);

    // Add to storage.
    auto room = RoomStorage::getRoomById(user.getRoomId());
    room->addObject(std::move(object));

    std::map<std::string, boost::json::value> params{
            {"positionX", boost::json::value(position.x)},
            {"positionY", boost::json::value(position.y)},
            {"dimensionX", boost::json::value(dimensions.x)},
            {"dimensionY", boost::json::value(dimensions.y)},
            {"filepath", boost::json::value(filepath)},
            {"id", boost::json::value(objectId)}};

    auto roomCommand = Command::createRoomCommand(user.getRoomId(), user, data, params, args.getMessageId());
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
