#include <nexilis/logger/log.hh>
#include <nexilis/server/command/command.hh>
#include <nexilis/server/command/commands.hh>
#include <nexilis/server/room_storage.hh>
#include <nexilis/util.hh>

namespace nexilis::server
{

CommandResult ServerImpl::room_player3d_setTeam(const DefaultArgs& args)
{
    auto& user = args.getUser();
    if (user.getRoomId() == 0)
    {
        Log::error("Set team: User not in room!");
        return CommandResult::error;
    }

    auto payload = Util::removeAmountOfBytesFromVector(args.getData(), 3);
    std::string team = Util::convertToString(payload);

    if (team != "Terrorist" && team != "Counter Terrorist")
    {
        Log::warning("Set team: Unknown team '", team, "' from user ", user.getId());
        return CommandResult::invalid_input;
    }

    auto room = RoomStorage::getRoomById(user.getRoomId());
    if (!room)
    {
        Log::error("Set team: Room not found");
        return CommandResult::failure;
    }

    room->setPlayerTeam(user.getId(), team);
    Log::info("Player ", user.getId(), " [", user.getUsername(), "] joined team ", team);

    // Announce the joining player's stats to the whole room so every client
    // can keep its local leaderboard table up to date...
    auto statsPayload = Command::createRoomLeaderboardCommand(
            room->getId(), user,
            Command::playerStatsEntries(*room, {user.getId()}), 0);
    room->broadcastToAll(statsPayload);

    // ...and seed the joining player with everyone currently in the room.
    auto fullPayload = Command::createRoomLeaderboardCommand(
            room->getId(), user,
            Command::playerStatsEntries(*room, room->getClients()),
            args.getMessageId());
    Command::sendMessageToClient(fullPayload, user, args.getProtocol());

    return CommandResult::success;
}

} // namespace nexilis::server
