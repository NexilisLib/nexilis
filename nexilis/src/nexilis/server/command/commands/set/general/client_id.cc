#include <nexilis/server/client_storage.hh>
#include <nexilis/server/command/commands.hh>

namespace nexilis::server
{

CommandResult ServerImpl::set_general_clientId(const DefaultArgs& args)
{
    Log::debug("setting::general::client_id");
    auto& user = args.getUser();
    auto data = args.getData();
    if (!user.hasRootAccess())
    {
        Log::error("Client needs root access for changing id");
        return CommandResult::unauthorized;
    }
    // We are parsing Command, so remove two bytes from this switch statement.
    auto payload = Util::removeAmountOfBytesFromVector(data, 3);
    uint64_t id = Util::convertToType<uint64_t>(payload);

    auto& clients = ClientStorage::getAllClients();

    for (auto c = clients.begin(); c != clients.end(); c++)
    {
        auto client = c->get();
        if (*client == user)
        {
            assert(client->hasRootAccess());
            assert(user.hasRootAccess());
            client->setId(id);
            return CommandResult::success;
        }
    }

    Log::error("Error in CommandType::set::clientID");
    return CommandResult::error;
}

} // namespace nexilis::server
