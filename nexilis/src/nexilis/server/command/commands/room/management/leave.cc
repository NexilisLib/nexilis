#include <nexilis/server/command/command.hh>
#include <nexilis/server/command/commands.hh>
#include <nexilis/server/room_storage.hh>

namespace nexilis::server
{

CommandResult ServerImpl::room_management_leave(const DefaultArgs& args)
{
    auto& user = args.getUser();
    auto* currentRoom = RoomStorage::getRoomById(user.getRoomId());

    if (!currentRoom)
    {
        Log::warning("Client not currently in room so cannot leave current room.");
        return CommandResult::failure;
    }

    currentRoom->leaveRoom(user.getId());
    assert(!RoomStorage::getRoomById(user.getRoomId())->contains(user.getId()));
    user.setRoomId(0);

    std::map<std::string, boost::json::value> params;
    auto roomCommand = Command::createRoomCommand(currentRoom->getId(), user, args.getData(), params, args.getMessageId());
    Command::sendRoomCommand(roomCommand, user, args.getProtocol());
    return CommandResult::success;
}

} // namespace nexilis::server
