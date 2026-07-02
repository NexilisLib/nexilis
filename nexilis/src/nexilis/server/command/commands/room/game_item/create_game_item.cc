#include <nexilis/object/game_item.hh>
#include <nexilis/server/command/command.hh>
#include <nexilis/server/command/commands.hh>
#include <nexilis/server/room_storage.hh>

#include <sstream>

namespace nexilis::server
{

CommandResult ServerImpl::room_gameitem_create(const DefaultArgs& args)
{
    auto& user = args.getUser();
    auto data = args.getData();
    auto payload = Util::removeAmountOfBytesFromVector(data, 3);

    auto position = Util::vector3fFromFront(payload);
    payload = Util::removeAmountOfBytesFromVector(payload, 12);
    auto dimensions = Util::vector3fFromFront(payload);
    payload = Util::removeAmountOfBytesFromVector(payload, 12);

    auto combined = Util::convertToString(payload);
    std::vector<std::string> parts;
    std::stringstream ss(combined);
    std::string part;
    while (std::getline(ss, part, '\0'))
    {
        parts.push_back(part);
    }

    std::string item_type = parts.size() > 0 ? parts[0] : "";
    std::string status = parts.size() > 1 ? parts[1] : "";
    std::string filepath = parts.size() > 2 ? parts[2] : "";

    auto item = GameItem(Util::getRandomUint64(), item_type, position, dimensions, status, filepath);
    uint64_t itemId = item.getId();
    Log::info("Created game item with id: ", itemId, " type: ", item_type);

    auto room = RoomStorage::getRoomById(user.getRoomId());
    room->addGameItem(std::move(item));

    std::map<std::string, boost::json::value> params{
            {"id", boost::json::value(itemId)},
            {"x", boost::json::value(position.x)},
            {"y", boost::json::value(position.y)},
            {"z", boost::json::value(position.z)},
            {"w", boost::json::value(dimensions.x)},
            {"h", boost::json::value(dimensions.y)},
            {"d", boost::json::value(dimensions.z)},
            {"item_type", boost::json::value(item_type)},
            {"status", boost::json::value(status)},
            {"filepath", boost::json::value(filepath)}};

    auto roomCommand = Command::createRoomCommand(user.getRoomId(), user, data, params, args.getMessageId());
    if (Command::sendRoomCommand(roomCommand, user, args.getProtocol()))
        return CommandResult::success;
    else
        return CommandResult::failed_room_send;
}

} // namespace nexilis::server
