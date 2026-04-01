#include <nexilis/room_data.hh>
#include <nexilis/server/command/command.hh>
#include <nexilis/server/command/commands.hh>
#include <nexilis/server/room.hh>
#include <nexilis/server/room_storage.hh>

namespace nexilis::server
{

CommandResult Commands::Room::Management::create(const DefaultArgs& args)
{
    auto payload = Util::removeAmountOfBytesFromVector(args.getData(), 3);
    uint8_t context = payload[0];
    std::string roomName = Util::convertToString(Util::removeAmountOfBytesFromVector(payload, 1));

    if (roomName.empty())
    {
        Log::error("Room name cannot be empty");
        return CommandResult::invalid_input;
    }
    else if (roomName == "")
    {
        Log::error("Room name cannot be an empty string");
        return CommandResult::invalid_input;
    }
    else if (roomName == " ")
    {
        Log::error("Room name cannot be equal to \" \" ");
        return CommandResult::invalid_input;
    }
    else if (roomName.length() > 20)
    {
        Log::error("Too long room name");
        return CommandResult::invalid_input;
    }
    else
    {
        auto room_data = RoomData(args.getUser().getId(), roomName, Util::getRandomUint64(), static_cast<RoomData::Context>(context));
        auto newRoom = nexilis::server::Room(room_data);

        auto newRoomId = newRoom.getId();
        RoomStorage::add(std::move(newRoom));

        std::map<std::string, boost::json::value> params;
        auto roomCommand = Command::createRoomCommand(newRoomId, args.getUser(), args.getData(), params, args.getMessageId());
        Command::sendRoomCommand(roomCommand, args.getUser(), args.getProtocol());
        return CommandResult::success;
    }
}

} // namespace nexilis::server
