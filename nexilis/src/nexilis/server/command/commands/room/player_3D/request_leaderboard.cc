#include <nexilis/command_type.hh>
#include <nexilis/logger/log.hh>
#include <nexilis/room_command_type.hh>
#include <nexilis/server/client_storage.hh>
#include <nexilis/server/command/command.hh>
#include <nexilis/server/command/commands.hh>
#include <nexilis/server/room_storage.hh>
#include <nexilis/util.hh>

#include <boost/json/array.hpp>
#include <boost/json/value.hpp>

#include <map>

namespace nexilis::server
{

CommandResult ServerImpl::room_player3d_request_leaderboard(const DefaultArgs& args)
{
    auto& user = args.getUser();
    if (user.getRoomId() == 0)
    {
        Log::error("request_leaderboard: User not in room!");
        return CommandResult::error;
    }

    auto room = RoomStorage::getRoomById(user.getRoomId());
    if (!room)
    {
        Log::error("request_leaderboard: Room not found");
        return CommandResult::failure;
    }

    boost::json::array entries;
    for (auto clientId : room->getClients())
    {
        auto* client = ClientStorage::getClientById(clientId);
        if (!client)
            continue;

        boost::json::object entry;
        entry["id"] = boost::json::value(clientId);
        entry["username"] = boost::json::value(client->getUsername());
        entry["team"] = boost::json::value(room->getPlayerTeam(clientId));
        entry["kills"] = boost::json::value(room->getPlayerKills(clientId));
        entry["deaths"] = boost::json::value(room->getPlayerDeaths(clientId));
        entries.emplace_back(std::move(entry));
    }

    std::map<std::string, boost::json::value> params{
            {"entries", boost::json::value(entries)}};

    nx_data leaderboardData;
    leaderboardData.emplace_back(static_cast<uint8_t>(CommandType::room));
    leaderboardData.emplace_back(static_cast<uint8_t>(RoomCommandType::Root::player_3D));
    leaderboardData.emplace_back(static_cast<uint8_t>(RoomCommandType::PlayerType::leaderboard));

    if (Command::sendRoomCommand(
                Command::createRoomCommand(
                        room->getId(), user, leaderboardData, params, args.getMessageId()),
                user, args.getProtocol()) == false)
        return CommandResult::failed_room_send;

    return CommandResult::success;
}

} // namespace nexilis::server