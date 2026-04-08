#include <nexilis/server/command/commands.hh>
#include <nexilis/server/server_json.hh>

nexilis::server::CommandResult nexilis::server::Commands::Get::Info::rooms(const DefaultArgs& args)
{
    return Commands::Get::Info::create(args, std::make_pair(ServerJson::getRoomData(), "room_data"));
}
