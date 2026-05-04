#include <nexilis/server/command/commands.hh>
#include <nexilis/server/server_json.hh>

namespace nexilis::server
{

CommandResult ServerImpl::get_info_clients(const DefaultArgs& args)
{
    return CommandResult::unimplemented;
    // return Commands::Get::Info::create(args, std::make_pair(ServerJson::getClientData(), "client_data"));
}

} // namespace nexilis::server
