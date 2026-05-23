#include <nexilis/server/client_storage.hh>
#include <nexilis/server/command/command.hh>
#include <nexilis/server/command/commands.hh>
#include <nexilis/server/room_storage.hh>

#include <algorithm>

namespace nexilis::server
{

CommandResult ServerImpl::room_management_remove(const DefaultArgs& args)
{
    auto payload = Util::removeAmountOfBytesFromVector(args.getData(), 3);
    uint64_t roomId = Util::convertToType<uint64_t>(payload);

    auto* room = RoomStorage::getRoomById(roomId);

    if (!room)
    {
        Log::error("Cannot find room with specified id!");
        return CommandResult::invalid_input;
    }

    // Only the creator can remove the room.
    if (room->getCreatorId() != args.getUser().getId())
    {
        Log::error("User does not have permission to remove room!");
        return CommandResult::unauthorized;
    }

    // Remove all clients from the room first.
    auto roomClients = room->getClients();
    for (auto clientId : roomClients)
    {
        auto* client = ClientStorage::getClientById(clientId);
        if (client)
        {
            client->setRoomId(0);
        }
    }

    // Remove the room from storage.
    auto& rooms = RoomStorage::getAllRooms();
    rooms.erase(std::remove_if(rooms.begin(), rooms.end(),
                               [roomId](const Room& r)
                               {
                                   return r.getId() == roomId;
                               }),
                rooms.end());

    std::map<std::string, boost::json::value> params{
            {"action", boost::json::value("remove")},
            {"roomId", boost::json::value(roomId)}};

    auto roomCommand = Command::createRoomCommand(roomId, args.getUser(), args.getData(), params, args.getMessageId());
    Command::sendRoomCommand(roomCommand, args.getUser(), args.getProtocol());

    return CommandResult::success;
}

} // namespace nexilis::server
