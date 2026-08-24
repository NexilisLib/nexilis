#include <nexilis/logger/log.hh>
#include <nexilis/server/command/command.hh>
#include <nexilis/server/command/commands.hh>
#include <nexilis/server/room_storage.hh>

namespace nexilis::server
{

CommandResult ServerImpl::room_management_setOverlap(const DefaultArgs& args)
{
    Log::debug("Commands::Room::Management::setOverlap", args);
    auto& user = args.getUser();

    auto payload = Util::removeAmountOfBytesFromVector(args.getData(), 3);
    if (payload.empty())
    {
        Log::error("Set overlap: Insufficient payload");
        return CommandResult::invalid_input;
    }
    bool allowed = payload.front() != 0;

    auto* room = RoomStorage::getRoomById(user.getRoomId());
    if (!room)
    {
        Log::error("Set overlap: User not in room!");
        return CommandResult::error;
    }

    room->setOverlappingAllowed(allowed);
    Log::info("Room ", room->getId(), " overlapping set to ", allowed);

    std::map<std::string, boost::json::value> params{
            {"overlap_allowed", boost::json::value(allowed)}};

    // Notify the room so peers can mirror the new overlap status.
    auto roomCommand = Command::createRoomCommand(room->getId(), user, args.getData(),
                                                  params, args.getMessageId());
    Command::sendRoomCommand(roomCommand, user, args.getProtocol());

    return CommandResult::success;
}

} // namespace nexilis::server
