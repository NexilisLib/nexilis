#ifndef NEXILIS_CLIENT_COMMAND_ROOM_LEADERBOARD_HH
#define NEXILIS_CLIENT_COMMAND_ROOM_LEADERBOARD_HH

#include <nexilis/client/base_api_command.hh>
#include <nexilis/client/client_api.hh>
#include <nexilis/util.hh>

#include <boost/json/array.hpp>
#include <boost/json/value.hpp>

#include <vector>

namespace nexilis::client
{

/// Stores a player stats update received from the server.
///
/// The server pushes these to keep clients' locally-maintained kill/death
/// tables current: a delta (killer + victim) is broadcast on every kill and
/// the joining player's entry is broadcast on team join, while a fresh joiner
/// receives a full seed of everyone's stats. The game merges the entries into
/// its local leaderboard instead of requesting a snapshot on demand.
class RoomLeaderboardCommand : public BaseAPICommand
{
public:
    explicit RoomLeaderboardCommand(const boost::json::array& entries)
    {
        for (const auto& item : entries)
        {
            if (!item.is_object())
                continue;

            const auto& obj = item.as_object();
            ClientAPI::LeaderboardEntry entry;

            if (obj.contains("id"))
                entry.id = Util::toUint64(obj.at("id"));
            if (obj.contains("username"))
                entry.username = obj.at("username").as_string().c_str();
            if (obj.contains("team"))
                entry.team = obj.at("team").as_string().c_str();
            if (obj.contains("kills"))
                entry.kills = Util::toUint64(obj.at("kills"));
            if (obj.contains("deaths"))
                entry.deaths = Util::toUint64(obj.at("deaths"));

            m_entries.emplace_back(std::move(entry));
        }
    }

    ReadResult execute(ClientAPI&, ClientAPI::ClientAPIData& data) override
    {
        ClientAPI::LeaderboardEvent event;
        event.entries = std::move(m_entries);
        data.pushLeaderboardEvent(std::move(event));
        return ReadResult::success;
    }

private:
    std::vector<ClientAPI::LeaderboardEntry> m_entries;
};

} // namespace nexilis::client

#endif
