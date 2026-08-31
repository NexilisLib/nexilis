#include <algorithm>

#include <nexilis/command_type.hh>
#include <nexilis/logger/log.hh>
#include <nexilis/room_command_type.hh>
#include <nexilis/server/command/command.hh>
#include <nexilis/server/command/commands.hh>
#include <nexilis/server/room_storage.hh>
#include <nexilis/util.hh>

namespace nexilis::server
{

CommandResult ServerImpl::room_player3d_shoot(const DefaultArgs& args)
{
    auto& user = args.getUser();
    if (user.getRoomId() == 0)
    {
        Log::error("Shoot: User not in room!");
        return CommandResult::error;
    }

    auto payload = Util::removeAmountOfBytesFromVector(args.getData(), 3);
    if (payload.size() < 12)
    {
        Log::error("Shoot: Insufficient payload");
        return CommandResult::invalid_input;
    }

    auto targetId = Util::uint64FromFront(payload);
    payload = Util::removeAmountOfBytesFromVector(payload, 8);
    auto damage = Util::floatFromFront(payload);

    auto room = RoomStorage::getRoomById(user.getRoomId());
    if (!room)
    {
        Log::error("Shoot: Room not found");
        return CommandResult::failure;
    }

    bool targetFound = std::any_of(room->getClients().begin(), room->getClients().end(),
                                   [targetId](auto clientId)
                                   { return clientId == targetId; });
    if (!targetFound)
    {
        Log::warning("Shoot: Target ", targetId, " not in room");
        return CommandResult::failure;
    }

    room->damagePlayer(targetId, damage);
    float newHealth = room->getPlayerHealth(targetId);

    Log::info("Player ", user.getId(), " shot player ", targetId,
              " for ", damage, " damage. HP: ", newHealth);

    std::map<std::string, boost::json::value> params{
            {"target_id", boost::json::value(targetId)},
            {"damage", boost::json::value(static_cast<double>(damage))},
            {"new_health", boost::json::value(static_cast<double>(newHealth))}};

    if (Command::sendRoomCommand(
                Command::createRoomCommand(
                        user.getRoomId(), user, args.getData(), params, args.getMessageId()),
                user, args.getProtocol()) == false)
        return CommandResult::failed_room_send;

    if (newHealth <= 0.0f)
    {
        room->resetPlayerHealth(targetId);

        std::map<std::string, boost::json::value> respawnParams{
                {"target_id", boost::json::value(targetId)}};

        nx_data respawnData;
        respawnData.emplace_back(static_cast<uint8_t>(nexilis::CommandType::room));
        respawnData.emplace_back(static_cast<uint8_t>(RoomCommandType::Root::player_3D));
        respawnData.emplace_back(static_cast<uint8_t>(RoomCommandType::PlayerType::respawn));

        if (!Command::sendRoomCommand(
                    Command::createRoomCommand(
                            user.getRoomId(), user, respawnData, respawnParams,
                            args.getMessageId()),
                    user, args.getProtocol()))
            return CommandResult::failed_room_send;
    }

    return CommandResult::success;
}

} // namespace nexilis::server
