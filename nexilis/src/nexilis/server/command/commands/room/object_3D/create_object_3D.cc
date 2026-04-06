#include <nexilis/object/object_3d.hh>
#include <nexilis/server/command/command.hh>
#include <nexilis/server/command/commands.hh>
#include <nexilis/server/room_storage.hh>

namespace nexilis::server
{

CommandResult Commands::Room::Object3D::create(const DefaultArgs& args)
{
    auto& user = args.getUser();
    auto data = args.getData();
    auto payload = Util::removeAmountOfBytesFromVector(data, 3);

    auto position = Util::vector3fFromFront(payload);
    auto dimensions = Util::vector3fFromFront(Util::removeAmountOfBytesFromVector(payload, 12));
    auto fileBytes = Util::removeAmountOfBytesFromVector(payload, 24);
    auto filepath = Util::convertToString(fileBytes);

    // Create server object.
    auto object = nexilis::Object3D(Util::getRandomUint64(), position, dimensions);
    object.setFilepath(filepath);
    uint64_t objectId = object.getId();
    Log::info("Created object with id: ", objectId);

    // Add to storage.
    auto room = RoomStorage::getRoomById(user.getRoomId());
    room->addObject(std::move(object));

    std::map<std::string, boost::json::value> params{
            {"positionX", boost::json::value(position.x)},
            {"positionY", boost::json::value(position.y)},
            {"positionZ", boost::json::value(position.z)},
            {"dimensionX", boost::json::value(dimensions.x)},
            {"dimensionY", boost::json::value(dimensions.y)},
            {"dimensionZ", boost::json::value(dimensions.z)},
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
