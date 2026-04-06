#include <nexilis/movement_type.hh>
#include <nexilis/object/object_2d.hh>
#include <nexilis/server/command/command.hh>
#include <nexilis/server/command/commands.hh>
#include <nexilis/server/movement.hh>
#include <nexilis/server/room_storage.hh>

namespace nexilis::server
{

CommandResult Commands::Room::Object2D::createMoving(const DefaultArgs& args)
{
    auto& user = args.getUser();
    auto data = args.getData();

    // Get messagedata.
    auto payload = Util::removeAmountOfBytesFromVector(data, 3);
    auto startingPosition = Util::vector2fFromFront(payload);
    auto dimensions = Util::vector2fFromFront(Util::removeAmountOfBytesFromVector(payload, 8));
    auto movement = Util::vector2fFromFront(Util::removeAmountOfBytesFromVector(payload, 16));
    auto deltaTime = Util::floatFromFront(Util::removeAmountOfBytesFromVector(payload, 24));
    auto movementType = static_cast<MovementType>(payload[28]);
    auto fileBytes = Util::removeAmountOfBytesFromVector(payload, 29);
    auto filepath = Util::convertToString(fileBytes);

    // Create new object.
    auto object = nexilis::Object2D(Util::getRandomUint64(), startingPosition, dimensions);
    object.setFilepath(filepath);
    uint64_t objectId = object.getId();
    Log::info("Created object with id: ", objectId);

    // Add newly created object to storage.
    auto room = RoomStorage::getRoomById(user.getRoomId());
    room->addObject(std::move(object));

    std::map<std::string, boost::json::value> params{
            {"createMovingType", boost::json::value("create")},
            {"positionX", boost::json::value(startingPosition.x)},
            {"positionY", boost::json::value(startingPosition.y)},
            {"dimensionX", boost::json::value(dimensions.x)},
            {"dimensionY", boost::json::value(dimensions.y)},
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

    auto base_move = std::make_unique<Movement2D>(MovementData(objectId, deltaTime, data, args.getMessageId()), movement, movementFunction);
    Movement::object2D(std::move(base_move), user, args.getProtocol()).detach();
    return CommandResult::success;
}

} // namespace nexilis::server
