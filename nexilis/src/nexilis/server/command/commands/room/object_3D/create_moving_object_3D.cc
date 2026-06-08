#include <nexilis/movement_type.hh>
#include <nexilis/object/object_3d.hh>
#include <nexilis/server/command/command.hh>
#include <nexilis/server/command/commands.hh>
#include <nexilis/server/movement.hh>
#include <nexilis/server/room_storage.hh>

namespace nexilis::server
{

CommandResult ServerImpl::room_object3d_createMoving(const DefaultArgs& args)
{
    auto& user = args.getUser();
    auto data = args.getData();

    // Get messagedata.
    auto payload = Util::removeAmountOfBytesFromVector(data, 3);
    auto startingPosition = Util::vector3fFromFront(payload);
    auto dimensions = Util::vector3fFromFront(Util::removeAmountOfBytesFromVector(payload, 12));
    auto movement = Util::vector3fFromFront(Util::removeAmountOfBytesFromVector(payload, 24));
    auto deltaTime = Util::floatFromFront(Util::removeAmountOfBytesFromVector(payload, 36));
    auto movementType = static_cast<MovementType>(payload[40]);
    auto fileBytes = Util::removeAmountOfBytesFromVector(payload, 41);
    auto filepath = Util::convertToString(fileBytes);

    // Create new object.
    auto object = nexilis::Object3D(Util::getRandomUint64(), startingPosition, dimensions);
    object.setFilepath(filepath);
    uint64_t objectId = object.getId();
    Log::info("Created object with id: ", objectId);

    // Add newly created object to storage.
    auto room = RoomStorage::getRoomById(user.getRoomId());
    room->addObject(std::move(object));

    std::map<std::string, boost::json::value> params{
            {"createMovingType", boost::json::value("create")},
            {"x", boost::json::value(startingPosition.x)},
            {"y", boost::json::value(startingPosition.y)},
            {"z", boost::json::value(startingPosition.z)},
            {"w", boost::json::value(dimensions.x)},
            {"h", boost::json::value(dimensions.y)},
            {"d", boost::json::value(dimensions.z)},
            {"filepath", boost::json::value(filepath)},
            {"id", boost::json::value(objectId)}};
    auto roomCommand = Command::createRoomCommand(user.getRoomId(), user, data, params, args.getMessageId());
    Command::sendRoomCommand(roomCommand, user, args.getProtocol());

    std::function<double(double, double)> movementFunction;
    switch (movementType)
    {
        case MovementType::eased:
            movementFunction = [](double progress, double totalDistance) -> double
            {
                return Movement::easing(progress, totalDistance);
            };
            break;
        case MovementType::linear:
            movementFunction = [](double progress, double totalDistance) -> double
            {
                return Movement::linear(progress, totalDistance);
            };
            break;
        default:
            Log::error("Undefined movement type!");
    }

    auto movement_data = MovementData(objectId, deltaTime, data, args.getMessageId());
    auto base_move = std::make_unique<Movement3D>(movement_data, movement, movementFunction);

    Movement::object3D(std::move(base_move), user, args.getProtocol()).detach();
    return CommandResult::success;
}

} // namespace nexilis::server
