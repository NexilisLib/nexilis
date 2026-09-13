#include <nexilis/logger/log.hh>
#include <nexilis/server/command/commands.hh>
#include <nexilis/server/room_storage.hh>
#include <nexilis/util.hh>

namespace nexilis::server
{

CommandResult ServerImpl::room_player3d_set_team(const DefaultArgs& args)
{
    auto& user = args.getUser();
    if (user.getRoomId() == 0)
    {
        Log::error("set_team: User not in room!");
        return CommandResult::error;
    }

    auto room = RoomStorage::getRoomById(user.getRoomId());
    if (!room)
    {
        Log::error("set_team: Room not found");
        return CommandResult::failure;
    }

    auto payload = Util::removeAmountOfBytesFromVector(args.getData(), 3);
    auto team = Util::convertToString(payload);

    room->setPlayerTeam(user.getId(), team);
    Log::info("Player ", user.getId(), " joined team: ", team);

    return CommandResult::success;
}

} // namespace nexilis::server