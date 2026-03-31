#include <nexilis/server/client_storage.hh>
#include <nexilis/server/command/commands.hh>

namespace nexilis::server
{

CommandResult Commands::Set::General::clientId(User& user, const nx_data& data)
{
    Log::debug("setting::general::client_id");
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
        if (*c == user)
        {
            assert(c->hasRootAccess());
            assert(user.hasRootAccess());
            c->setId(id);
            return CommandResult::success;
        }
    }

    Log::error("Error in CommandType::set::clientID");
    return CommandResult::error;
}

} // namespace nexilis::server
