#include <nexilis/object/game_item.hh>
#include <nexilis/server/client_storage.hh>
#include <nexilis/server/command/command.hh>
#include <nexilis/server/command/commands.hh>
#include <nexilis/server/room_storage.hh>

namespace nexilis::server
{

CommandResult ServerImpl::room_management_join(const DefaultArgs& args)
{
    Log::debug("Commands::Room::Management::join", args);
    auto user_id = args.getUser().getId();

    auto payload = Util::removeAmountOfBytesFromVector(args.getData(), 3);
    uint64_t roomId = Util::convertToType<uint64_t>(payload);
    auto* room = RoomStorage::getRoomById(roomId);

    if (!room)
    {
        Log::error("Cannot find room with specified id!");
        return CommandResult::invalid_input;
    }

    if (RoomStorage::getRoomById(roomId)->contains(user_id))
    {
        Log::error("Cannot join room where the client already is!");
        return CommandResult::error;
    }

    room->joinRoom(user_id);

    args.getUser().setRoomId(room->getId());
    assert(ClientStorage::getClientById(user_id)->getRoomId() == room->getId());
    assert(RoomStorage::getRoomById(roomId)->contains(user_id));
    assert(args.getUser().getRoomId() == roomId);

    std::map<std::string, boost::json::value> params{{"type", boost::json::value("join")}};

    auto roomCommand = Command::createRoomCommand(roomId, args.getUser(), args.getData(), params, args.getMessageId());
    Command::sendRoomCommand(roomCommand, args.getUser(), args.getProtocol());

    // Send existing game items to the joining client
    auto& user = args.getUser();
    auto& protocol = args.getProtocol();
    for (auto& item : room->getGameItems())
    {
        auto pos = item.getPosition();
        auto dim = item.getDimensions();
        std::map<std::string, boost::json::value> itemParams{
                {"action", boost::json::value("create")},
                {"room_id", boost::json::value(roomId)},
                {"client_id", boost::json::value(user_id)},
                {"id", boost::json::value(item.getId())},
                {"x", boost::json::value(pos.x)},
                {"y", boost::json::value(pos.y)},
                {"z", boost::json::value(pos.z)},
                {"w", boost::json::value(dim.x)},
                {"h", boost::json::value(dim.y)},
                {"d", boost::json::value(dim.z)},
                {"item_type", boost::json::value(item.getType())},
                {"status", boost::json::value(item.getStatus())},
                {"filepath", boost::json::value(item.getFilepath())}};
        auto itemMsg = Command::clientMessageData(CommandType::room, "game_item", 0, itemParams);
        Command::sendMessageToClient(itemMsg, user, protocol);
    }

    return CommandResult::success;
}

} // namespace nexilis::server
