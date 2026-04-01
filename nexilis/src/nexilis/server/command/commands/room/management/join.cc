#include <nexilis/server/client_storage.hh>
#include <nexilis/server/command/command.hh>
#include <nexilis/server/command/commands.hh>
#include <nexilis/server/room_storage.hh>

namespace nexilis::server
{

CommandResult Commands::Room::Management::join(const DefaultArgs& args)
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
    // This does the exact same as,
    // ClientStorage::getClientById(user_id)->setRoomId(room->getId());
    // Proof:
    assert(ClientStorage::getClientById(user_id)->getRoomId() == room->getId());
    assert(RoomStorage::getRoomById(roomId)->contains(user_id));
    assert(args.getUser().getRoomId() == roomId);

    std::map<std::string, boost::json::value> params;
    auto roomCommand = Command::createRoomCommand(roomId, args.getUser(), args.getData(), params, args.getMessageId());
    Command::sendRoomCommand(roomCommand, args.getUser(), args.getProtocol());
    return CommandResult::success;
}

} // namespace nexilis::server
